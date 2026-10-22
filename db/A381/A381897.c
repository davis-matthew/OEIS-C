/*
 * A381897  a(n) = least integer m >= 2 such that prime(n) is a sum of the
 *          form Sum_{k>=0} floor(h/m^k) for some integer h >= 1.
 * https://oeis.org/A381897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer m ≥ 2 such that prime(n) can be written as
 *            Sum_{k≥0} floor(h / m^k) for some integer h ≥ 1. The function
 *            implements a simple search over m and a small range of h around
 *            an approximate value. The search always finds an m because at
 *            worst m = prime(n)−1 will work. The algorithm...
 * Range:     u8 n <= 45, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Get the k‑th prime, 1‑based indexing. */
#include "../utils/get_prime.h"
#include "../utils/floor_sum.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* prime(n) */
    for (int m = 2; ; ++m) {            /* search m */
        seq_t num     = (seq_t)p * (m - 1);
        seq_t h0      = num / m;        /* rough h ≈ p*(m-1)/m */
        seq_t low     = (h0 > 100) ? h0 - 100 : 1;
        seq_t high    = h0 + 100;       /* try a window of ±100 */

        for (seq_t h = low; h <= high; ++h) {
            if (floor_sum(h, m) == p) {
                return (seq_t)m;        /* found minimal m */
            }
        }
        /* If not found, continue to next m.  The loop will eventually
           terminate because at worst a very large m will succeed. */
    }
}
