/*
 * A358094  a(n) is the number of ways n can be reached in the following
 *          method: we start with 1, then add or multiply alternately, and
 *          each operand must be 2 or 3.
 * https://oeis.org/A358094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4217, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358094: a(n) = number of ways to reach n by alternating add/multiply
   operations with operands 2 or 3, starting from 1.
   The sequence is computed via dynamic programming:
   • add[v] = ways to reach value v with the last operation being addition
              (next must be multiplication).
   • mul[v] = ways to reach value v with the last operation being multiplication
              (next must be addition).
   The recurrence is:
       add[v]  -> mul[2*v]  += add[v]
                   mul[3*v]  += add[v]
       mul[v]  -> add[v+2]  += mul[v]
                   add[v+3]  += mul[v]
   The base step from 1 produces the first entries into add[3], add[4],
   mul[2], mul[3].  The answer is add[n] + mul[n] (with a(1)=1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1ULL) return 0;
    if (n == 1ULL) return 1;

    /* Allocate arrays for values up to n */
    size_t sz = (size_t)n + 1;
    seq_t *add = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *mul = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!add || !mul) {
        /* Memory allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    /* First step from starting value 1 */
    if (3ULL <= n) add[3] += 1;   /* 1 + 2 = 3  (last op addition) */
    if (4ULL <= n) add[4] += 1;   /* 1 + 3 = 4  (last op addition) */
    if (2ULL <= n) mul[2] += 1;   /* 1 * 2 = 2  (last op multiplication) */
    if (3ULL <= n) mul[3] += 1;   /* 1 * 3 = 3  (last op multiplication) */

    /* Dynamic programming */
    for (idx_t v = 1ULL; v <= n; ++v) {
        seq_t valAdd = add[v];
        if (valAdd) {
            if (v <= n / 2ULL) mul[v * 2ULL] += valAdd;
            if (v <= n / 3ULL) mul[v * 3ULL] += valAdd;
        }
        seq_t valMul = mul[v];
        if (valMul) {
            if (v <= n - 2ULL) add[v + 2ULL] += valMul;
            if (v <= n - 3ULL) add[v + 3ULL] += valMul;
        }
    }

    seq_t result = add[n] + mul[n];
    free(add);
    free(mul);
    return result;
}
