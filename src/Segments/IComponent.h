#pragma once

namespace segments{
    class IComponent {
    public:
    virtual void Init() = 0; // Call at the setup method
    virtual void Update() = 0; // Rapidly call in loop method
    virtual ~IComponent() {}
    };

}
