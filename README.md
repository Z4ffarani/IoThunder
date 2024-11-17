![banner](./assets/banner.png)

![simulator](./assets/simulator.png)

## LINKS
- **[SIMULADOR](https://wokwi.com/projects/411876289650649089)**

<br>

# PROJETO
Este projeto visa conectar um Simulador (montado no site Wokwi) a uma instância EC2 (máquina virtual da Amazon Web Services) com o sistema operacional Ubuntu e um repositório do GitHub clonado, que simplifica serviços da plataforma de código aberto FIWARE. O repositório apresenta contêineres Docker (aplicações de código isoladas), e para o sistema foi utilizado o Mosquitto (broker MQTT) diretamente de um contêiner Docker, viabilizando a comunicação com o Simulador por meio do aplicativo MyMQTT.

A estrutura do sistema IoT é baseada em três pilares principais:

- **Simulador** | Conecta sensores e atuadores ao ESP32 (microcontrolador) e transmite os dados via protocolo de comunicação MQTT para o EC2.

- **EC2** | Máquina virtual, com o Ubuntu instalado, que atua como servidor para manipular e enviar dados recebidos do Simulador por meio do Mosquitto.

- **MyMQTT** | Aplicativo que se inscreve ou publica em tópicos MQTT (previamente estabelecidos) por meio da conexão IP (Internet Protocol) do EC2 com o Simulador, permitindo o controle de seus sensores e atuadores.

<br>

# INSTRUÇÕES
1. Em um terminal, clonar o repositório:
```bash
git clone https://github.com/Z4ffarani/IoThunder.git
```

2. No terminal, navegar até a pasta do projeto:
```bash
cd IoThunder
```

3. O documento `criacao-servidor-e-maquina-virtual`, localizado na pasta `docs`, informa o passo a passo necessário para criar uma máquina virtual (podendo ser tanto localmente quanto em nuvem por meio do provedor de infraestrutura de preferência), instalar o Ubuntu (distribuição Linux de preferência) e clonar o repositório do GitHub com serviços do FIWARE, configurando-os em contêineres Docker (obrigatório).

4. Para rodar o Simulador, seguir o link disponibilizado e clicar no botão de iniciar. Levará algum tempo até que o Simulador se conecte à rede pública do Wokwi e ao EC2. É possível regular os níveis de condição relacionados aos sensores clicando neles, para que assim sejam registrados no EC2 e enviados para o MyMQTT. Para utilização do Simulador físico, é necessário executar o código presente no arquivo `simulator.ino` na pasta `docs`, assim como instalar as bibliotecas dos sensores presentes na pasta `libraries`. O programa [Arduino IDE](https://www.arduino.cc/en/software) é adequado para o teste.

5. Instalar o aplicativo [MyMQTT](https://mymqtt.app/en), ou qualquer outro que permita a conexão de endereço IP e porta 1883 de uma máquina virtual. No aplicativo, caso a mensagem de publicação (Publish) no tópico MQTT `iot/listen` seja `led/on`, o LED liga no Simulador e desliga com `led/off`. Se a mensagem no mesmo tópico for `buzzer/high`, o Buzzer é acionado com uma frequência aguda, e com `buzzer/low`, uma frequência grave, ambas em um intervalo de 500 milissegundos.

<br>

# FUNÇÕES
`docs/simulator.ino`:
- **Inclusão de bibliotecas** | Importa as bibliotecas necessárias para a operação do ESP32 e a comunicação com os sensores, permitindo o uso de funções específicas para cada tipo de sensor conectado.

- **Definição de portas de entrada** | Estabelece as portas digitais e analógicas do ESP32 que serão usadas para conectar e ler os dados dos sensores, garantindo a correta configuração do hardware.

- **Referenciamento e leitura dos sensores** | Configura e lê os dados dos sensores DHT22 e HC-SR04, processando as informações de temperatura, umidade e distância.

- **Constantes de endereços** | Recebe valores que permitem a comunicação do sistema, como rede Wi-Fi, endereço IP da máquina virtual e porta do broker MQTT.

- **Constantes de tópicos MQTT** | Recebe rotas que permitirão a comunicação com o broker MQTT através de inscrição e publicação de informações.

- **Funções de inicialização** | Inicia o módulo Wi-Fi do ESP32, o broker MQTT, juntamente com seus futuros tópicos e sensores do Simulador.

- **Funções de publicação** | Permitem enviar dados dos sensores através de seus respectivos tópicos para a máquina virtual.

- **Callback MQTT** | O broker MQTT traz mensagens inscritas em um tópico no MyMQTT para o Simulador. A função as interpreta por meio de códigos condicionais e executa as ações nos atuadores relacionados ao tópico.

- **Funções de verificação em loop** | Verifica a conexão Wi-Fi do Simulador e a conexão com o broker MQTT, além de confirmar o status com mensagens na Serial do Simulador.

<br>

# DIAGRAMA
![DIAGRAM](./assets/diagram.png)

<br>

# MONTAGEM DO SIMULADOR
- **1x** Cabo USB A - **R$15,00**
- **1x** ESP32-DevKitC V4 [**(Documentação)**](https://docs.espressif.com/projects/arduino-esp32/en/latest/) - **R$50,00**
- **1x** Sensor de distância ultrassônico HC-SR04 [**(Documentação)**](https://web.eece.maine.edu/zhu/book/lab/HC-SR04%20User%20Manual.pdf) - **R$13,00**
- **1x** Sensor de umidade e temperatura (DHT22) [**(Documentação)**](https://www.mouser.com/datasheet/2/737/dht-932870.pdf?srsltid=AfmBOorSkF-0gQ-mg5eU7CV1wMu6Tfkj42UYvsNNHSDjhMDF8vXLC82A) - **R$40,00**
- **1x** LED verde difuso de 5mm - **R$0,50**
- **1x** Piezoeletric buzzer ativo de 5V [**(Documentação)**](https://wiki-content.arduino.cc/documents/datasheets/PIEZO-PKM22EPPH4001-BO.pdf) - **R$4,00**
- **1x** Resistor de 220 Ω (ohms) - **R$2,00**
- **1x** Resistor de 200 Ω (ohms) - **R$2,00**
- **1x** Placa de ensaio (de preferência com 830 pontos) - **R$20,00**
- **15x** Cabos Jumper macho-macho (contados com a placa de ensaio incluída) - **R$3,00**
- **TOTAL - R$149,50**

<br>

# OBSERVAÇÕES 
- Os preços dos componentes do Simulador estão sujeitos a valorização ou desvalorização.

- Para o Simulador físico, o uso contínuo de sensores pode resultar em um maior consumo de energia. É recomendável implementar modos de economia de energia, como intervalos de sono, para prolongar a vida útil da bateria, se o sistema for alimentado por bateria.

- A estabilidade da conexão Wi-Fi é fundamental para o funcionamento do sistema. Para o Simulador físico, verifique se o módulo ESP32 está dentro do alcance da rede sem fio para garantir a transmissão confiável de dados.

- As bibliotecas escolhidas para o Simulador têm recursos ainda mais complexos para serem implementados.

- É necessário alterar o endereço IP e porta do broker MQTT no Simulador para que ele se conecte à máquina virtual escolhida.

- É essencial configurar o Mosquitto corretamente na máquina virtual para permitir a comunicação entre o Simulador e o MyMQTT. Testes prévios com o Postman ajudarão a validar dependências instaladas.

- Existe a possibilidade de renomear e alterar as funcionalidades dos tópicos MQTT no Simulador para atender a diferentes necessidades na aplicação IoT, bem como os sensores e atuadores dispostos.

- Para a máquina virtual em nuvem, dependendo do provedor de infraestrutura escolhido, é importante averiguar valores e planos gratuitos que podem se tornar pagos antes de ativar instâncias. O término da instância após testes é recomendado para evitar cobranças inesperadas.

- O EC2 utilizado no projeto permanecerá parado, sendo necessário criar uma máquina virtual rodando em outro provedor para uma aplicação IoT própria.

- O endereço IP de uma máquina virtual em nuvem se altera uma vez que é parada, necessitando alterá-lo no Simulador e no MyMQTT quando reativada.

<br>

# TECNOLOGIAS
[![Arduino](https://img.shields.io/badge/-Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/en/software)
[![AWS](https://img.shields.io/badge/AWS-%23FF9900.svg?style=for-the-badge&logo=amazon-aws&logoColor=white)](https://aws.amazon.com)
[![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)](https://ubuntu.com)

<br>

# REFERÊNCIAS
- **[fiware](https://github.com/fabiocabrini/fiware)**
- **[Instalação de bibliotecas no Arduino IDE](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/)**
- **[How to Create an EC2 Instance in AWS in 2023](https://www.youtube.com/watch?v=0Gz-PUnEUF0&t=526s)**
- **[Aula 1 - Configuração de um dispositivo no Fiware](https://www.youtube.com/watch?v=8oHkAlXdWo8)**

<br>

# AGRADECIMENTOS
- **[Paulo Marcotti](https://www.youtube.com/@pmarcotti)**
- **[Fábio Henrique Cabrini](https://github.com/fabiocabrini)**
