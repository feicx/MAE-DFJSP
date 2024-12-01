# MAE-DFJSP

This project is the source code of an improved two-individual-based evolutionary algorithm for the flexible job shop scheduling problem with machine-worker dual constraints, please read following instruction carefully. The parameter, input and output all are included in it.

## Adjustment of MAE Parameters

The parameters in the MAE algorithm are defined in `const.h` in the source file directory, and the following table explains the meaning of each constant. After completing the changes, you can recompile to complete the update.

| Constants         | Meaning                                                                 |
|-------------------|-------------------------------------------------------------------------|
| `max_time`        | Maximum Runtime of the MAE Algorithm in Seconds                        |
| `P`               | The algebra of a cycle in the MAE algorithm.                          |
| `alpha`           | Parameter α in Path Relinking                                          |
| `beta`            | Parameter β in Path Relinking                                          |
| `gamma`           | Parameter γ in Path Relinking                                          |
| `t_retain`        | Maximum Size of Solution Retention Mechanism                           |
| `t_gen`           | Maximum Iteration Number in Tabu Search                                |
| `t_neighbor_num`  | Number of Neighbors Generated in One Iteration in Tabu Search          |
| `t_swap`          | Number of Perturbations in Neighbor of Operation-based Swaped Neighbourhood Structure                           |
| `t_insert`        | Number of Perturbations in Neighbor of Operation-based Insertion Neighbourhood Structure                           |
| `t_machine`       | Number of Perturbations in Neighbor of Machine-based Neighbourhood Structure                           |
| `t_worker`        | Number of Perturbations in Neighbor of Worker-based Neighbourhood Structure                          |

## Program Running Interface

![Program Interface](/assert/running_interface.png)

## Guidelines for Using MAE

The MAE Framework inputs the data from the file `input.txt`.

![Input Format](/assert/input_format.png)

It needs to be entered in the following format:  

1. On the first line, enter three numbers for the number of jobs $n$, the number of machines $m$, and the number of workers $l$. The machines and workers are numbered $1 \sim m$ and $1 \sim l$.  
2. Next, enter the information for each job in turn.

### Job Format

The format of each job $j_i$ is as follows:  

- The first line of the job is the total number of operations for this job, $n_\text{job}$.  
- Next, enter the information for each operation $o$ in turn.  

### Operation Format

The format of each operation $o_i$ is as follows:  

1. The first line is entered as an integer indicating the amount of time this operation will take.  
2. Next, enter a number representing the number of candidate machines for this operation, followed by each candidate machine on each line in the format $m_i$ and $factor$, which is the number of the machine and the influence factor, respectively.  
3. Next, enter a number representing the number of candidate worker machines for this operation, followed by individual candidate workers on each line in the candidate machine format $w_i$ and $factor$, the number of the worker and the influencing factor, respectively.  

### Input Notes

- Each input is an integer except for $factor$, which accepts double-precision floating-point numbers.  
- Each input is separated by a space or a newline.  

### Output

Two files, `solution.csv` and `log.txt`, are output when the program is completed:  

- `solution.csv`: Contains the optimal solution produced at the end of the algorithm.  
- `log.txt`: Records the algorithm's runtime.  

It is worth noting that the output job serial numbers are numbered according to the order in which you entered the jobs, from $1$ to $n$ completion.

![Program Interface](/assert/running_output.png)

