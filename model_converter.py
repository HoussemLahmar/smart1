import joblib
import numpy as np
import os

# Optionnel : conversion vers ONNX, TensorFlow Lite, etc.
# Ici, conversion vers ONNX avec skl2onnx si besoin

try:
    from skl2onnx import convert_sklearn
    from skl2onnx.common.data_types import FloatTensorType
except ImportError:
    convert_sklearn = None

MODEL_PATH = "zone_pump_model.pkl"
ONNX_PATH = "zone_pump_model.onnx"

model = joblib.load(MODEL_PATH)

if convert_sklearn is not None:
    initial_type = [("float_input", FloatTensorType([None, 4]))]
    onnx_model = convert_sklearn(model, initial_types=initial_type)
    with open(ONNX_PATH, "wb") as f:
        f.write(onnx_model.SerializeToString())
    print(f"Modèle converti en ONNX ({ONNX_PATH})")
else:
    print("skl2onnx non installé, conversion ONNX non réalisée.")

# Pour TensorFlow Lite, il faudrait exporter en Keras ou utiliser un modèle TF natif.