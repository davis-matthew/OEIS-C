/*
 * A247453  T(n,k) = binomial(n,k)*A000111(n-k)*(-1)^(n-k), 0 <= k <= n.
 * https://oeis.org/A247453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where
 *            T(r,k)=binomial(r,k)*A000111(r-k)*(-1)^(r-k), * read row‑wise
 *            from the triangle (row 0, row 1, …)
 * Range:     i8 n <= 27, i16 n <= 54, i32 n <= 119, i64 n <= 299,
 *            i128 n <= 779, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t euler_cache[20000];
static idx_t euler_max = 0;

static void ensure_euler_up_to(idx_t n)
{
    if (euler_max == 0) {
        euler_cache[0] = 1;
        euler_cache[1] = 1;
        euler_max = 1;
    }
    if (n <= euler_max) return;

    for (idx_t i = euler_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t t = binomial(i - 1, k);
            t = t * euler_cache[k];
            t = t * euler_cache[i - 1 - k];
            sum = sum + t;
        }
        euler_cache[i] = sum >> 1; /* divide by 2 */
    }
    euler_max = n;
}

seq_t a(idx_t n)
{
    idx_t r = 0, cum = 0;
    while (cum + r + 1 <= n) {
        cum += r + 1;
        ++r;
    }
    idx_t k = n - cum;
    idx_t m = r - k;

    ensure_euler_up_to(m);

    seq_t sign = ((m & 1) ? (seq_t)-1 : (seq_t)1);
    seq_t bin  = binomial(r, k);
    seq_t prod = bin * euler_cache[m];
    return prod * sign;
}
