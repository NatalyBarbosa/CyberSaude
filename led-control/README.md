# IEEE Cyber Saúde 🩺💡

Aplicativo mobile desenvolvido para controlar LEDs conectados a um **ESP32**, utilizando comunicação Wi-Fi através de requisições HTTP.

O projeto integra desenvolvimento mobile e sistemas embarcados, permitindo controlar individualmente três LEDs por meio de uma interface simples e intuitiva.

## 🚀 Tecnologias utilizadas

### Aplicativo Mobile
- [React Native](https://reactnative.dev/) — desenvolvimento da interface mobile.
- [Expo](https://expo.dev/) — ambiente de desenvolvimento.
- **Expo SDK 58** — versão do SDK utilizada.
- [Expo Go](https://expo.dev/go) — execução do aplicativo durante o desenvolvimento.
- JavaScript — linguagem de programação.

### Hardware e Comunicação
- ESP32 — microcontrolador.
- Arduino IDE — programação do ESP32.
- C++ — linguagem utilizada no firmware.
- Wi-Fi — conexão entre o aplicativo e o ESP32.
- HTTP — comunicação entre o aplicativo e o servidor embarcado.

## 🔌 Componentes e conexões

O projeto utiliza três LEDs conectados ao ESP32.

| Componente | Pino GPIO |
|---|---:|
| LED Vermelho | GPIO 21 |
| LED Amarelo | GPIO 5 |
| LED Verde | GPIO 4 |

**Observação:** utilize resistores adequados em série com os LEDs para limitar a corrente elétrica.

## 📱 Funcionalidades

- Controle individual do LED vermelho.
- Controle individual do LED amarelo.
- Controle individual do LED verde.
- Acionamento e desligamento através de interruptores (Switch).
- Comunicação entre aplicativo e ESP32 via Wi-Fi.
- Requisições HTTP para controlar os LEDs.
- Feedback de erro em caso de falha na comunicação.

## 📡 Comunicação entre aplicativo e ESP32

O ESP32 funciona como um servidor HTTP, disponibilizando rotas para controlar os LEDs.

| Método | Endpoint | Ação |
|---|---|---|
| GET | `/` | Verificar servidor |
| GET | `/vermelho/ligar` | Ligar LED vermelho |
| GET | `/vermelho/desligar` | Desligar LED vermelho |
| GET | `/amarelo/ligar` | Ligar LED amarelo |
| GET | `/amarelo/desligar` | Desligar LED amarelo |
| GET | `/verde/ligar` | Ligar LED verde |
| GET | `/verde/desligar` | Desligar LED verde |

O aplicativo realiza as requisições utilizando `fetch()`.

**Importante:** o endereço IP utilizado no aplicativo deve corresponder ao IP recebido pelo ESP32 na conexão Wi-Fi.

O celular e o ESP32 precisam estar conectados à mesma rede local.

## ⚙️ Como executar o projeto

### 1. Pré-requisitos

- Node.js
- pnpm ou npm
- Expo Go
- Arduino IDE
- ESP32
- Cabo USB para programação do ESP32

### 2. Clonar o repositório

```bash
git clone URL_DO_REPOSITORIO
```

Entre na pasta:

```bash
cd NOME_DO_PROJETO
```

### 3. Instalar as dependências

Com pnpm:

```bash
pnpm install
```

Ou com npm:

```bash
npm install
```

### 4. Configurar o ESP32

Abra o código do ESP32 na Arduino IDE e configure as credenciais da sua rede Wi-Fi:

```cpp
const char* ssid = "NOME_DO_WIFI";
const char* password = "SENHA_DO_WIFI";
```

Carregue o código no ESP32.

Abra o Monitor Serial na velocidade de `115200` baud para visualizar o endereço IP recebido.

### 5. Configurar o aplicativo

No código React Native, atualize o endereço utilizado nas requisições HTTP:

```javascript
const resposta = await fetch(
  `http://IP_DO_ESP32/${led}/${acao}`
);
```

Substitua `IP_DO_ESP32` pelo endereço apresentado no Monitor Serial.

### 6. Executar o aplicativo

```bash
npx expo start
```

Abra o Expo Go no celular e leia o QR Code apresentado no terminal.

## 📥 Download do Expo Go

Instale o Expo Go para executar o aplicativo durante o desenvolvimento:

- [Download oficial do Expo Go](https://expo.dev/go)

## 📂 Organização do projeto

```text
projeto/
├── assets/
│   └── images/
│       ├── ieee.jpeg
│       └── cyber.jpeg
│
├── src/
│   └── app/
│       └── (tabs)/
│           └── index.js
│
├── components/
├── package.json
└── README.md
```

*A estrutura acima é ilustrativa e pode ser ajustada conforme a organização atual do projeto.*

## 🔒 Segurança

- Não compartilhe credenciais da rede Wi-Fi.
- Não publique senhas no repositório.
- Utilize o ESP32 somente em uma rede confiável.
- O servidor HTTP deste projeto não possui autenticação nem criptografia.

## 👨‍💻 Desenvolvimento

Projeto desenvolvido no contexto do **IEEE Cyber Saúde**, integrando desenvolvimento mobile, eletrônica e comunicação sem fio.

**IEEE Cyber Saúde** — Tecnologia conectando soluções.
