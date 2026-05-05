# check_onnx_output.py
import onnxruntime as ort
import numpy as np

# 加载ONNX模型
session = ort.InferenceSession('best.onnx')

# 查看输入输出信息
print("输入信息:")
for input in session.get_inputs():
    print(f"  名称: {input.name}, 形状: {input.shape}, 类型: {input.type}")

print("\n输出信息:")
for output in session.get_outputs():
    print(f"  名称: {output.name}, 形状: {output.shape}, 类型: {output.type}")

# 创建测试输入
input_name = session.get_inputs()[0].name
dummy_input = np.random.randn(1, 3, 640, 640).astype(np.float32)

# 运行推理
outputs = session.run(None, {input_name: dummy_input})

print("\n推理结果:")
for i, output in enumerate(outputs):
    print(f"输出 {i} 形状: {output.shape}")
    
    # 如果是 [1, 25200, 6] 格式
    if len(output.shape) == 3 and output.shape[1] == 25200 and output.shape[2] >= 4:
        data = output[0]  # 取第一个batch
        
        print(f"前5个预测:")
        for j in range(5):
            print(f"  预测 {j}: ", end="")
            for k in range(min(6, output.shape[2])):
                print(f"{data[j, k]:.6f} ", end="")
            print()
        
        # 分析数值范围
        print(f"\n值范围分析:")
        print(f"  第0列范围: {data[:100, 0].min():.3f} 到 {data[:100, 0].max():.3f}")
        print(f"  第1列范围: {data[:100, 1].min():.3f} 到 {data[:100, 1].max():.3f}")
        print(f"  第2列范围: {data[:100, 2].min():.3f} 到 {data[:100, 2].max():.3f}")
        print(f"  第3列范围: {data[:100, 3].min():.3f} 到 {data[:100, 3].max():.3f}")
        
        # 判断格式
        if data[:100, 0].min() >= 0 and data[:100, 0].max() <= 1:
            print("  → 可能是归一化坐标 (0-1)")
        elif data[:100, 2].min() >= 0 and data[:100, 2].max() <= 640:
            print("  → 可能是像素坐标")
        else:
            print("  → 可能是原始YOLO输出 (需要解码)")