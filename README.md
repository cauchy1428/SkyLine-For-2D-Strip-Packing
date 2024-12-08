# SkyLine For 2D Strip Packing

## 2D Strip Packing
2D Strip Packing is a classic optimization problem in computer science and operations research. The goal is to pack a set of rectangles (items) into a strip of fixed width (WIDTH) such that the total height of the packed rectangles is minimized.

## Algorithm: SKYLINE
The **SKYLINE** algorithm is a heuristic method designed to efficiently solve the 2D Strip Packing problem. The key idea is to define a struct `space` to represent potential areas where rectangles can be placed. By evaluating the **fitness** of each `space`, the algorithm selects the most suitable position to insert each rectangle. 

For more details, please refer to the paper:  
[An efficient skyline heuristic for the 2D Strip Packing Problem](https://www.sciencedirect.com/science/article/abs/pii/S037722171100508X?fr=RR-2&ref=pdf_download&rr=8eed4bfdc8188945)

## Input and Output Description
- **Input**:
  1. An integer `n`, representing the number of rectangles (items).
  2. An integer `WIDTH`, representing the fixed width of the strip's base.
  3. `n` integers, representing the widths of the rectangles.
  4. `n` integers, representing the heights of the rectangles.

- **Output**:
  - A single integer, representing the minimum height required to pack all rectangles into the strip.

## Debug Mode
To assist in understanding and debugging the algorithm, a `DEBUG` mode has been implemented:
- Uncomment the macro definition `#define DEBUG` in the code to enable DEBUG mode.
- In DEBUG mode, the program will print detailed information about each step of the process and intermediate results to the console.

## Improvements
- In this implementation, I used **C++ STL's `vector`** to record the `spaces` and `Rectangles`. While `vector` is a flexible and easy-to-use data structure, using other specialized data structures such as **heaps**, **balanced trees**, or **priority queues** may speed up the algorithm.


## Feedback and Bug Reporting
Your feedback is highly appreciated! If you find any bugs or have suggestions for improvement, please let me know.
