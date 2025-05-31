#include "ml_inference.h"
#include "smartirrigation_model.h" // modèle TFLite Micro auto-généré

// -- Librairie TFLite Micro --
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Mémoire statique micro
constexpr int kTensorArenaSize = 8 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

// Entrées/sorties selon modèle (.tflite)
InferenceResult runModelIA(const CapteursZone& capteurs) {
    // 1. Setup modèle
    const tflite::Model* model = tflite::GetModel(smartirrigation_model_tflite);
    tflite::MicroAllOpsResolver resolver;
    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, kTensorArenaSize);
    interpreter.AllocateTensors();

    // 2. Remplir entrée
    TfLiteTensor* input = interpreter.input(0);
    input->data.f[0] = capteurs.humidite_sol;
    input->data.f[1] = capteurs.ec_sol;
    input->data.f[2] = capteurs.temp_sol;
    input->data.f[3] = capteurs.ldr1;
    input->data.f[4] = capteurs.ldr2;

    // 3. Inference
    interpreter.Invoke();

    // 4. Récupérer sortie
    TfLiteTensor* output = interpreter.output(0);
    InferenceResult res;
    res.irriguer = output->data.f[0] > 0.5;
    res.duree_minutes = min((uint8_t)output->data.f[1], IRRIGATION_MAX_MINUTES);
    res.priorite = (output->data.f[2] > 0.7) ? "Urgent" : (output->data.f[2] > 0.3 ? "Normal" : "Faible");
    return res;
}