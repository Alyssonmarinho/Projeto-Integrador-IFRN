# Detector de Cores Sonoro para Pessoas com Deficiência Visual

Protótipo de um detector de cores com retorno sonoro, desenvolvido na disciplina de **Projeto Integrador** do curso Técnico em Manutenção e Suporte em Informática do **IFRN – Campus Natal Central (CNAT)**. O dispositivo foi criado ao longo de 2024 por uma equipe de estudantes, aplicando conhecimentos técnicos em hardware, software e eletrônica, além de habilidades de colaboração.

## Objetivo

Desenvolver um dispositivo capaz de identificar cores e informar o resultado ao usuário por meio de áudio, promovendo maior **autonomia e inclusão** para pessoas com deficiência visual.

## Demonstração

### Vídeo do protótipo

[![Vídeo do protótipo funcionando](https://img.youtube.com/vi/cjDdaL6z20M/maxresdefault.jpg)](https://www.youtube.com/watch?v=cjDdaL6z20M)

### Apresentação

Os slides explicando o projeto estão disponíveis [neste link](https://docs.google.com/presentation/d/1XsQCt_nyhYkyqQ3EESrcrQCo_cW-yKOF43W0hZdBB_U/edit).

## Como funciona

1. O sensor de cor TCS230 lê a cor do objeto.
2. O Arduino identifica a cor a partir dos valores lidos e da calibração.
3. O módulo DFPlayer Mini toca o áudio correspondente, armazenado em um cartão microSD.
4. Ao apertar o botão, o Arduino combina as duas últimas cores primárias detectadas e toca o áudio da cor resultante.

### Cores detectadas

Branco, vermelho, verde, azul e amarelo.

### Misturas de cores

| Cor 1    | Cor 2   | Resultado |
|----------|---------|-----------|
| Vermelho | Amarelo | Laranja   |
| Azul     | Amarelo | Verde     |
| Vermelho | Azul    | Roxo      |

Qualquer outra combinação, ou o uso do botão sem duas cores detectadas, toca o áudio de mistura inválida.

## Tecnologias e componentes

- Plataforma Arduino (linguagem C/C++)
- Sensor de cor TCS3200 (TCS230)
- Módulo de áudio DFPlayer Mini
- Cartão microSD com os arquivos de áudio
- Alto-falante
- Botão (push button)
- Protoboard e jumpers
- Bibliotecas: `SoftwareSerial` e `DFRobotDFPlayerMini`

## Ligações

### Sensor de cor TCS3200

| Pino do sensor | Pino do Arduino |
|----------------|-----------------|
| S0             | 4               |
| S1             | 5               |
| S2             | 6               |
| S3             | 7               |
| OUT            | 8               |
| VCC            | 5V              |
| GND            | GND             |

### Módulo DFPlayer Mini

| Pino do DFPlayer | Ligação                        |
|------------------|--------------------------------|
| VCC              | 5V do Arduino                  |
| GND              | GND do Arduino                 |
| RX               | Pino 11 do Arduino             |
| TX               | Pino 10 do Arduino             |
| SPK_1            | Um terminal do alto-falante    |
| SPK_2            | Outro terminal do alto-falante |

O alto-falante é ligado diretamente nos pinos **SPK_1** e **SPK_2** do DFPlayer, em qualquer ordem. Não ligue nenhum deles no GND. O DFPlayer Mini suporta alto-falantes de até 3 W.

### Botão

| Pino do botão  | Ligação            |
|----------------|--------------------|
| Um terminal    | Pino 13 do Arduino |
| Outro terminal | GND                |

O botão usa o resistor pull-up interno do Arduino, então não precisa de resistor externo. Também é recomendado usar um resistor de 1 kΩ no pino RX do DFPlayer.

## Como rodar o código no Arduino

### 1. Instalar a IDE e a biblioteca

1. Baixe e instale a [IDE do Arduino](https://www.arduino.cc/en/software).
2. Abra a IDE e vá em **Sketch → Include Library → Manage Libraries...**
3. Busque por **DFRobotDFPlayerMini** e clique em **Install**.

A biblioteca `SoftwareSerial` já vem instalada com a IDE.

### 2. Preparar o cartão microSD

1. Formate o cartão em **FAT32**.
2. Os áudios estão na pasta [`audios/`](audios/) deste repositório. Copie os arquivos MP3 para a **raiz** do cartão, sem a pasta, mantendo os nomes.
3. Se quiser usar áudios próprios, nomeie cada arquivo com quatro dígitos, seguindo a numeração da tabela abaixo.

| Arquivo    | Áudio            |
|------------|------------------|
| `0001.mp3` | Azul             |
| `0002.mp3` | Amarelo          |
| `0003.mp3` | Vermelho         |
| `0004.mp3` | Laranja          |
| `0005.mp3` | Roxo             |
| `0006.mp3` | Verde            |
| `0007.mp3` | Preto            |
| `0008.mp3` | Branco           |
| `0009.mp3` | Cor desconhecida |
| `0011.mp3` | Mistura inválida |
| `0012.mp3` | Mistura: laranja |
| `0013.mp3` | Mistura: roxo    |
| `0014.mp3` | Mistura: verde   |

Depois, coloque o cartão no DFPlayer Mini.

### 3. Montar o circuito

Faça as ligações conforme as tabelas da seção **Ligações**, incluindo o alto-falante nos pinos SPK_1 e SPK_2 do DFPlayer Mini.

### 4. Enviar o código para a placa

1. Clone o repositório ou baixe o arquivo `detector_de_cores.ino`.
2. Abra o arquivo na IDE do Arduino. Se a IDE perguntar, aceite criar uma pasta com o mesmo nome do arquivo.
3. Conecte o Arduino ao computador pelo cabo USB.
4. Em **Tools → Board**, selecione a sua placa (por exemplo, **Arduino Uno**).
5. Em **Tools → Port**, selecione a porta em que o Arduino está conectado.
6. Clique em **Upload** (botão com a seta para a direita).

### 5. Testar

1. Abra o **Serial Monitor** (**Tools → Serial Monitor**) com a velocidade em **9600 baud**.
2. Aproxime um objeto colorido do sensor. O nome da cor aparece no monitor e o áudio é tocado.
3. Para misturar cores, detecte duas cores diferentes em sequência e aperte o botão.

### Problemas comuns

- **"Erro ao iniciar MP3 Player" no monitor serial:** confira se o cartão SD está colocado, formatado em FAT32 e se as ligações RX e TX do DFPlayer estão corretas.
- **Cores identificadas errado:** ajuste a calibração, descrita abaixo.
- **Sem som:** verifique o alto-falante nos pinos SPK_1 e SPK_2, o volume (definido como 20 no código) e os nomes dos arquivos no cartão.

## Calibração

Os valores `redMin`, `redMax`, `greenMin`, `greenMax`, `blueMin` e `blueMax` no início do código dependem do sensor e da iluminação do ambiente. Se as cores estiverem sendo identificadas incorretamente, ajuste esses valores para as suas condições.

## Habilidades desenvolvidas

O projeto envolveu o desenvolvimento de circuitos eletrônicos, programação embarcada em C/C++, leitura e calibração de sensores, processamento de sinais, controle do módulo DFPlayer Mini, comunicação serial e testes iterativos para garantir o funcionamento correto do protótipo. A equipe também aplicou metodologias de planejamento e documentação técnica, fortalecendo a resolução de problemas, o trabalho em equipe e o desenvolvimento de sistemas embarcados.

## Resultado

O protótipo final demonstra o potencial de tecnologias assistivas de baixo custo, evidenciando a aplicação prática da programação em C, da eletrônica aplicada e da integração entre hardware e software para promover acessibilidade e inovação social.

## Autores

- Alysson Felipe Viana Marinho
- Gabriel da Rocha Sousa
- João Inô Ferreira Barbosa
- Wagner Lucas Lima Santos