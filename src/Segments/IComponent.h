#pragma once

namespace segments{
    class IComponent {
    public:
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual ~IComponent() {}
    };

}
