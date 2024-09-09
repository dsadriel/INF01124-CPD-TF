# Aplicação de B-Tree na Implementação de um Protótipo de Agendamentos para Clínicas

**Autores**: Adriel de Souza e Clara Schons Theisen

**Orientador**: Prof. Dr. Leandro Krug Wives

---

## Descrição do Projeto
Este projeto consiste em um protótipo de sistema gerenciador de agendamentos para uma clínica médica, desenvolvido como parte do trabalho final da disciplina de Classificação e Pesquisa de Dados da UFRGS. O objetivo principal é aplicar os conceitos de estruturas de dados, como B-Tree, e gerenciamento de bancos de dados em um cenário realista.

A aplicação permite a importação, padronização, armazenamento e consulta de dados relacionados a agendamentos e pacientes. O dataset utilizado foi o **"Medical Appointment No Shows"** da plataforma Kaggle, contendo cerca de 110.000 entradas.

Além disso, um dataset adicional foi criado para armazenar informações de pacientes, como nomes aleatórios gerados para enriquecer os dados.

### Armazenamento e Busca de Dados
- **B-Trees (Árvores B):** Utilizadas para otimizar a busca e a recuperação rápida de informações por meio de Arquivos de Índices, baseados em uma implementação adaptada de *Comer (1979)*.
- **Arquivos Seriais:** Utilizados para armazenar os dados em formato binário, facilitando a persistência e manipulação dos registros.

A aplicação oferece cinco módulos principais:
1. **Importação**
2. **Criação**
3. **Listagem**
4. **Consulta**
5. **Edição**

Esses módulos permitem o processamento incremental dos dados, garantindo a integridade e atualização contínua do banco de dados à medida que novas informações são adicionadas ou alteradas.

## Guia de Compilação e Execução

### Requisitos
- **GCC (GNU Compiler Collection)**

### Compilação
Cada módulo da aplicação pode ser compilado individualmente no terminal. Execute o seguinte comando a partir do diretório raiz do projeto:

```
gcc -std=c11 -I include -L lib -o <nome_do_arquivo> lib/* src/<nome_do_arquivo>.c
```

Substitua `<nome_do_arquivo>` pelo nome do módulo desejado. Certifique-se de que todos os arquivos necessários estão presentes e que a estrutura de diretórios está correta.

Alternativamente, use o arquivo batch `compilar-todos.bat` para compilar todos os módulos de uma vez.

### Execução
Após a compilação, os módulos podem ser executados a partir do terminal:

```
.\<nome_do_modulo> [argumentos]
```

Substitua `<nome_do_modulo>` pelo nome do módulo que você deseja executar e inclua os parâmetros necessários no lugar de `[argumentos]`.

## Considerações Finais
Este projeto teve como objetivo principal a implementação de um protótipo - ou prova de conceito - de um sistema de gestão de agendamentos e pacientes de uma clínica médica fictícia, utilizando estruturas de dados e técnicas para persistência de dados. Através do desenvolvimento desta aplicação, tivemos a oportunidade de aplicar conceitos vistos na disciplina de Classificação de Pesquisa de Dados, em especial, Árvores B, Arquivos Seriais e Normalização de Tabelas Relacionais.

O trabalho apresentou desafios interessantes, que foram acentuados devido à escolha de uma linguagem de baixo nível. As implementações da estrutura de dados e algoritmos requereram conhecimentos adquiridos não somente por meio desta disciplina, mas também em disciplinas anteriores a ela.

Infelizmente, devido a limitações de tempo, não foi possível implementar as estruturas de Arquivos Invertidos e Árvores TRIE/PATRICIA, que permitiriam realizar buscas mais eficientes nos registros a partir de atributos secundários, como o nome do paciente ou data do agendamento. Estas funcionalidades teriam complementado o sistema oferecendo uma maior flexibilidade e agilidade na busca de registros específicos ou de consultas de forma mais genérica.

Considerando outras possíveis melhorias, uma futura adição de uma interface gráfica ao projeto facilitaria a interação dos usuários com o sistema, dado que o uso da aplicação através da linha de comando pode ser menos intuitivo e requerer um conhecimento prévio do funcionamento de uso de um terminal de comandos.

## Referências
- Kaggle, "Medical Appointment No Shows".
- Comer, D. (1979). "Ubiquitous B-Trees". *Computing Surveys*.

---

Veja o relatório completo em [relatorio.pdf](relatorio.pdf) e o código fonte em [src/](src/).