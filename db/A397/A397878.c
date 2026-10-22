/*
 * A397878  Number of integers of the form r^k (0 < r < n, k > 0) whose base-n
 *          alternating digital root is r and whose number of base-n digits is
 *          k.
 * https://oeis.org/A397878
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of pairs (r,k) with 1 ≤ r < n, k ≥ 1, such that *
 *            n^{k-1} ≤ r^k < n^k and r^k mod (n+1) = r. The trivial solutions
 *            * (k = 1) give the base count of n−1
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

static seq_t mod_pow_uint(idx_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    seq_t m = mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % m;
        b = (b * b) % m;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* value outside OEIS offset */
    seq_t total = (seq_t)(n - 1);   /* trivial k = 1 solutions */

    idx_t mod = n + 1;
    idx_t limit = n - 1;
    double ln_n = log((double)n);

    for (idx_t k = 2; ; ++k) {
        double exp_frac = ((double)(k - 1)) / k;            /* (k‑1)/k   */
        double pow_val = pow((double)n, exp_frac);          /* n^(k-1)/n */
        idx_t r = (idx_t)floor(pow_val + 1e-12);            /* safety */
        if (r < 1) r = 1;

        /* correct possible downward rounding */
        while ((double)k * log((double)r) < ((double)k - 1.0) * ln_n)
            ++r;

        if (r > limit) break;    /* no r satisfy digit condition for this k */

        for (idx_t cur = r; cur <= limit; ++cur) {
            if (mod_pow_uint(cur, k, mod) == cur)
                ++total;
        }
    }
    return total;
}
