/*
 * A244149  a(n) = 2*(n*denominator(((n-1)*(n^2)+2^(n+1)-4)/(2*n))-n)/n+1.
 * https://oeis.org/A244149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n/gcd(((n-1)*n^2 + 2^(n+1) - 4) , 2*n) - 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1950, u64 n <= 9770,
 *            u128 n <= 9770, big any n
 * Helpers:   gcd, mod_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* sequence undefined for n = 0 */

    seq_t mod = (seq_t)2 * (seq_t)n;              /* 2*n */
    seq_t term1 = ((seq_t)n - 1) * (seq_t)n * (seq_t)n; /* (n-1)*n^2 */
    if (mod != 0) term1 %= mod;

    seq_t term2 = mod_pow2((seq_t)n + 1, mod);    /* 2^(n+1) mod 2n */
    seq_t term4 = 4 % mod;                        /* 4 mod 2n */

    seq_t N_mod = (term1 + term2) % mod;
    N_mod = (N_mod + mod - term4) % mod;          /* ((term1+term2) - 4) mod mod */

    seq_t g = gcd(N_mod, mod);                    /* gcd(N, 2*n) */

    seq_t ans = (seq_t)4 * (seq_t)n / g - 1;       /* 4*n/g - 1 */
    return ans;
}
