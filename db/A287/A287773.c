/*
 * A287773  {0->010, 1->101}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 1 0 1 … obtained by mapping the infinite Fibonacci word
 *            A003849 with 0→010 and 1→101 (i.e. a(n) is the (n‑1) mod 3 th
 *            bit of the mapped word)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_word_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------- */
/*  Because this file is compiled both as C11 and C++17 we cast  */
/*  every malloc/realloc result explicitly.                      */
/* ------------------------------------------------------------- */
#include <stdlib.h>
#include "../utils/fib_word_bit.h"

/* ------------------------------------------------------------- */
/* Helper: return the n'th (0‑based) bit of the infinite        */
/* Fibonacci word A003849 (0‑basis).  This word can be defined   */
/* recursively: w₀=0, w₁=01, w_{k}=w_{k-1}w_{k-2}.            */

/* ------------------------------------------------------------- */
/* Main sequence function.                                        */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is at least 1.  Compute the corresponding position in    */
    /* the infinite Fibonacci word and the offset inside the      */
    /* 3‑bit block.                                               */
    idx_t pos    = (n - 1) / 3;      /* 0‑based index in Fibonacci word   */
    idx_t offset = (n - 1) % 3;      /* 0, 1 or 2                         */

    seq_t bit = fib_word_bit(pos);   /* 0 or 1 from Fibonacci word       */

    /* Mapping tables.  Bit 0 → 010, bit 1 → 101.                */
    static const seq_t map0[3] = {0, 1, 0};
    static const seq_t map1[3] = {1, 0, 1};

    return (bit == 0) ? map0[(int)offset] : map1[(int)offset];
}
