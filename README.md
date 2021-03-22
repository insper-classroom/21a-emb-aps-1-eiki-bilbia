
<!-- PROJECT LOGO -->
<br />
<p align="center">
  <h3 align="center">Atividade Prática Supervisionada 1</h3>

  <p align="center">
    Computacao Embarcada
    <br />
    <a href="https://github.com/insper-classroom/21a-emb-aps-1-eiki-bilbia"><strong>Explore os Arquivos »</strong></a>
    <br />
    <br />
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Sumário</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">Sobre o Projeto</a>
    </li>
    <li>
      <a href="#getting-started">Infraestrutura</a>
    </li>
    <li><a href="#usage">Projeto</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## Sobre o Projeto

O objetivo do projeto é reproduzir músicas monofônicas, que são aquelas que só possuem uma única nota tocada por vez. Todas as músicas foram obtidas em um repositório apontado pelos professores, cujo link está mais abaixo. O projeto toca 4 músicas diferentes, e você pode circular entre elas clicando nos botões do OLED, além de tocar a música, pausar e parar. O projeto também consta com leds que piscam de acordo com a frequência de cada nota e uma barra de progresso de cada música.


<!-- GETTING STARTED -->
## Infraestrutura

Para criar o dispositivo embarcado utilizou-se o microcontrolador ATSAME70Q21, um ARM Cortex M7.

<img src="img/same70.png">

 Foi utilizado
o OLED1 Xplained Pro, para permitir a comunicação com o usuário através da tela e dos três botões embutidos na extensão.
Um buzzer também foi utilizado para a reprodução das notas musicais. 

<img src="img/oled1.jpg">

## Ligações elétricas

Para montar a placa, deve-se encaixar o OLED1 nos pinos EXT1 da placa Atmel:  

## Projeto

Para reproduzir as músicas monofônicas com o buzzer, foi necessário implementar uma função que controla a frequência das
vibrações e considere também o tempo de duração de cada nota. 


<!-- CONTRIBUTING -->
## Video

[Video de Demonstração no Youtube](https://www.youtube.com/watch?v=4Uk_XwgxECI "Video Demo")



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.


<!-- CONTACT -->
## Alunos

Eiki Luis Yamashiro: [GitHub](github.com/EikiYamashiro/), eikily@al.insper.edu.br

Beatriz Muniz: [GitHub](https://github.com/Bilbia/), beatrizmcs@al.insper.edu.br

