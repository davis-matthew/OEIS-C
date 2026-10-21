/*
 * A262881  Regular triangle where the n-th row lists the integers k between 0
 *          and n ordered by increasing value of the Hamming weight of k, and
 *          if equal by increasing value of k.
 * https://oeis.org/A262881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2079, u16 n <= 2079, u32 n <= 2079, u64 n <= 2079,
 *            u128 n <= 2079, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A262881:  
   Regular triangle where the n-th row lists integers k (0..n) ordered  
   by increasing Hamming weight of k and then by k. The sequence is  
   produced row by row starting from n = 0. */
#include <stdint.h>
#include <math.h>

/* popcount for uint64_t (no built‑in dependency) */
static inline uint64_t popcount_uint64(uint64_t x)
{
    uint64_t c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;

    /* Find row r such that r*(r+1)/2 <= N < (r+1)*(r+2)/2 */
    uint64_t r = (uint64_t)((sqrt((double)(8.0 * N + 1.0)) - 1.0) / 2.0);
    /* Correct potential rounding errors */
    if (r * (r + 1) / 2 > N) {
        while (r * (r + 1) / 2 > N) --r;
    } else {
        while ((r + 1) * (r + 2) / 2 <= N) ++r;
    }

    uint64_t start = r * (r + 1) / 2;
    uint64_t pos   = N - start;          /* 0‑based position inside the row */

    /* Count numbers 0..r by popcount */
    uint64_t cnt[65] = {0};
    for (uint64_t k = 0; k <= r; ++k) {
        uint64_t pc = popcount_uint64(k);
        ++cnt[pc];
    }

    /* Find the popcount group that contains the desired position */
    uint64_t cumulative = 0;
    uint64_t target_pc  = 0;
    for (uint64_t pc = 0; pc <= 64; ++pc) {
        if (cumulative + cnt[pc] > pos) {
            target_pc = pc;
            break;
        }
        cumulative += cnt[pc];
    }

    uint64_t inside = pos - cumulative;   /* position inside the chosen group */
    uint64_t seen   = 0;
    uint64_t result = 0;
    for (uint64_t k = 0; k <= r; ++k) {
        if (popcount_uint64(k) == target_pc) {
            if (seen == inside) {
                result = k;
                break;
            }
            ++seen;
        }
    }

    return (seq_t)result;
}
