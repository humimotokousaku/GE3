#pragma once
#include "DirectXCommon.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <dxcapi.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

class LinePSO
{
public:
	///
	/// Default Method
	///

	static LinePSO* GetInstance();

	~LinePSO() = default;

	// エンジンの初期化
	void Initialize();

	// 描画前の処理
	void PreDraw();

	// 描画後の処理
	void PostDraw();

	///
	/// User Method
	/// 

	// Getter
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState_; }
	Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature_; }

private:// プライベートな関数
	// DXCの初期化
	void DXCInitialize();

	// シェーダのコンパイル
	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

	// DescriptorRangeの生成
	void CreateDescriptorRange();

	// Samplerの設定
	void SettingSampler();

	// RootSignatureの生成
	void CreateRootSignature();

	// RootParameter生成
	void CreateRootParameter();

	// InputLayerの設定
	void SettingInputLayout();

	// BlendStateの設定
	void SettingBlendState();

	// RasterizerStateの設定
	void SettingRasterizerState();

	// ピクセルシェーダー
	void PixelSharder();

	// 頂点シェーダー
	void VertexSharder();

	// PSOの生成
	void CreatePSO();

	// PSO
	void PSO();

	// ビューポート
	void CreateViewport();

	// シザー矩形
	void CreateScissor();

private:
	IDxcUtils* dxcUtils_;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;

	ID3DBlob* errorBlob_;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[1];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_;
	ID3DBlob* signatureBlob_;

	D3D12_RASTERIZER_DESC rasterizerDesc_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescs_;
	D3D12_BLEND_DESC blendDesc_;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	D3D12_ROOT_PARAMETER rootParameters_[3];
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1];

	D3D12_STATIC_SAMPLER_DESC staticSamplers_[1];

	// 通常
	IDxcBlob* vertexShaderBlob_;
	IDxcBlob* pixelShaderBlob_;

	D3D12_VIEWPORT viewport_;
	D3D12_RECT scissorRect_;
};