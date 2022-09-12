#include "../../Common/Header/CommunicationUnit.h"

namespace TCP{
    class ClientUnit: public CommunicationUnit{
    private:
        std::string last_msg;
    public:
        ClientUnit();
        explicit ClientUnit(uint16_t port, uint32_t host = INADDR_ANY);
        int Disconnect() override;
        void LoadData() override;
        std::string_view GetLastMsg() const;
        bool operator == (const ClientUnit& rhs);
    };
}