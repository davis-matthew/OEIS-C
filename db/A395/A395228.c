/*
 * A395228  a(n) = number of 4-tuples (w, x, y, z) such that w*x + y*z = n,
 *          where w, x, y, z are positive Fibonacci numbers satisfying w < x <
 *          y < z.
 * https://oeis.org/A395228
 *
 * Offset:    17    (a(17) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A395228: number of increasing 4‑tuples of Fibonacci numbers (w,x,y,z)
   with w*x + y*z = n.  The function is naively loops over all
   quadruples; the number of Fibonacci numbers ≤ n is < 30 for n ≤ 10⁴,
   so the 4‑nested loops run in well under the time limit. */

#include <stdint.h>   /* for uint64_t, but idx_t and seq_t are defined by the build */

seq_t a(idx_t n)
{
    /* Behaviour for n < 17 is irrelevant – return 0. */
    if (n < 17) return (seq_t)0;

    /* Generate Fibonacci numbers (strictly increasing sequence 1,2,3,5,…) */
    /* The array is fixed‑size – 64 entries are enough for any practical n. */
    const idx_t MAX_F = 64;
    seq_t fib[MAX_F];
    idx_t cnt = 0;

    seq_t f0 = (seq_t)1;
    seq_t f1 = (seq_t)2;
    while (cnt < MAX_F && f0 <= n) {
        fib[cnt++] = f0;
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }

    if (cnt < 4) return (seq_t)0;   /* fewer than 4 distinct Fibonacci numbers */

    unsigned long long target = (unsigned long long)n;
    seq_t res = (seq_t)0;

    for (idx_t i = 0; i + 3 < cnt; ++i) {
        unsigned long long wi = (unsigned long long)fib[i];
        for (idx_t j = i + 1; j + 2 < cnt; ++j) {
            unsigned long long prod1 = wi * (unsigned long long)fib[j];
            if (prod1 > target) break;   /* further j only increase prod1 */

            for (idx_t k = j + 1; k + 1 < cnt; ++k) {
                unsigned long long y = (unsigned long long)fib[k];
                for (idx_t l = k + 1; l < cnt; ++l) {
                    unsigned long long prod2 = y * (unsigned long long)fib[l];
                    if (prod1 + prod2 == target) ++res;
                }
            }
        }
    }
    return res;
}
