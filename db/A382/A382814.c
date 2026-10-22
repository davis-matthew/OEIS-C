/*
 * A382814  Number of nachos that the first player gets when playing the
 *          "Fibonachos" game starting with n nachos.
 * https://oeis.org/A382814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of nachos taken by the first player in the
 * Fibonachos game with n tokens.
 *
 * Computation: Simulate the game.  Players alternate turns taking
 * Fibonacci-number amounts.  Whenever the remaining number of tokens
 * is less than the next Fibonacci number to be taken, the Fibonacci
 * sequence is restarted (i.e., the current player takes 1 token).
 * The game ends when all tokens are taken.  The function returns the
 * total taken by the first player.
 *
 * The algorithm runs in O(number_of_turns) time, which is bounded
 * by n, and uses only integer arithmetic.  It works for any n ≥ 1
 * and any seq_t/idx_t types provided by the build system, including
 * large-enough integer types or arbitrary-precision integers.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t total   = 0;           /* total nachos taken by player 1 */
    seq_t remaining = (seq_t)n;  /* remaining nachos (seq_t for precision) */
    idx_t fib_idx = 1;           /* current Fibonacci index (1‑based) */
    idx_t first_player = 0;      /* 0 → first player, 1 → second player */

    while (remaining > 0) {
        seq_t take = fib(fib_idx);          /* Fibonacci number for this turn */
        if (take > remaining) {             /* reset the sequence */
            fib_idx = 1;
            take = fib(fib_idx);
        }
        remaining -= take;                  /* take the tokens */

        if (first_player == 0)              /* accumulate for first player */
            total += take;

        first_player ^= 1;                  /* switch player */
        fib_idx++;                          /* advance Fibonacci index */
    }
    return total;
}
