/*
 * A341241  Expansion of (-1 + Product_{k>=1} 1 / (1 + (-x)^k))^3.
 * https://oeis.org/A341241
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is coefficient of x^n in (-1 + ∏_{k≥1} 1/(1+(-x)^k))^3 The
 *            sequence starts at offset 3: a(3)=1, a(4)=0, …
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Round‑up maximum n that the implementation supports.  The OEIS
   tests use n≈10000, so a modest buffer is sufficient. */
#define MAX_N 20000

/* Static buffers – one for the coefficients of the product
   P(x)=∏_{k≥1} 1/(1+(-x)^k) and one for the intermediate
   convolution.  They are reused between calls. */
static seq_t prod_coeff[MAX_N + 1];
static seq_t conv_coeff[MAX_N + 1];

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;                 /* values below the offset are irrelevant */

    if (n > MAX_N)                       /* just in case the caller wants a larger n */
        return (seq_t)0;                 /* safe‑guard: not expected to occur */

    /* Initialise coefficient arrays.  prod_coeff[0] = 1 represents the constant term
       of the product.  All other entries are zero.  conv_coeff is cleared later. */
    prod_coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        prod_coeff[(size_t)i] = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i)
        conv_coeff[(size_t)i] = (seq_t)0;

    /* Compute the coefficients of the infinite product P(x) up to x^n.
       The factor for k is 1/(1+(-x)^k) = 1/(1 - r_k x^k) with
       r_k = (-1)^(k+1).  For odd k, r_k = +1, for even k, r_k = -1.
       The standard unbounded‑knapsack DP works:   c[j] += r_k * c[j-k]   */
    for (idx_t k = 1; k <= n; ++k) {
        if (k & 1ULL) {   /* odd k => coefficient +1 */
            for (idx_t j = k; j <= n; ++j)
                prod_coeff[(size_t)j] += prod_coeff[(size_t)(j - k)];
        } else {          /* even k => coefficient -1 */
            for (idx_t j = k; j <= n; ++j)
                prod_coeff[(size_t)j] -= prod_coeff[(size_t)(j - k)];
        }
    }

    /* conv_coeff[s] = Σ_{i=1}^{s-1} prod_coeff[i] * prod_coeff[s-i]  (ordered pairs) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ei = prod_coeff[(size_t)i];
        for (idx_t j = 1; j + i <= n; ++j) {
            conv_coeff[(size_t)(i + j)] += ei * prod_coeff[(size_t)j];
        }
    }

    /* a(n) = coefficient of (∑_{i≥1} prod_coeff[i] x^i)^3
             = Σ_{k=1}^{n-2} prod_coeff[k] * conv_coeff[n-k] */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n - 2; ++k)
        result += prod_coeff[(size_t)k] * conv_coeff[(size_t)(n - k)];

    return result;
}
