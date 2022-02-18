#include "DrakEngine/Core/LayerStack.h"

namespace DrakEngine {
    LayerStack::~LayerStack() {
        for(auto layer : m_Layers) {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer *layer) {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsert, layer);
        m_LayerInsert++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer *overlay) {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer *layer) {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if(it != m_Layers.end()) {
            layer->OnDetach();
            m_Layers.erase(it);
            m_LayerInsert--;
        }
    }

    void LayerStack::PopOverlay(Layer *overlay) {
        auto  it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if(it != m_Layers.end()) {
            overlay->OnDetach();
            m_Layers.erase(it);
        }
    }
}
