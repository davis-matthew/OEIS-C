/*
 * A336914  Number of steps to reach 1 in '3^x+1' problem (a variation of the
 *          Collatz problem), or -1 if 1 is never reached.
 * https://oeis.org/A336914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of steps to reach 1 in the 3^x+1 map.
   The map:
     odd  → 3^x + 1              (step 1)
     even → floor(log₂(x))        (step 2)
   The function returns the total number of steps. */
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"          /* unsigned bitlen_seq_t(seq_t x) */

/*  LOG2(3) ≈ 1.584962500721156
    Using a rational approximation:
          LOG2(3) ≈ 1584962500721156 / 1000000000000000
    The error is < 10⁻¹², so floor(x*LOG2(3)) is exact for x ≤ 10⁴
    (the value range used in the tests). */
static const unsigned long long LOG2_3_NUM = 1584962500721156ULL;
static const unsigned long long LOG2_3_DEN = 1000000000000000ULL;

/* floor(x * LOG2(3)) for x up to 2⁶⁴ – uses 128‑bit arithmetic. */
static inline seq_t floor_log2_3pow(idx_t x)
{
    unsigned __int128 prod = (unsigned __int128)(unsigned long long)x * LOG2_3_NUM;
    unsigned long long res  = (unsigned long long)(prod / LOG2_3_DEN);
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    seq_t steps = 0;
    seq_t x     = (seq_t)n;

    while (x != (seq_t)1) {
        if ((x & (seq_t)1) == (seq_t)0) {          /* even */
            steps++;
            x = (seq_t)(bitlen_seq_t(x) - 1);      /* floor(log₂(x)) */
        } else {                                  /* odd  */
            steps += 2;                           /* odd → 3^x+1 → log₂(...) */
            x = floor_log2_3pow((idx_t)x);
        }
    }
    return steps;
}
