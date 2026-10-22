/*
 * A394772  G.f. A(x) satisfies A(x) = 1 - x * d/dx log(1 - x*A(x)^2).
 * https://oeis.org/A394772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n>=1: a(n) = (n-1)*∑_{k=0}^{n-1} a(k)*a(n-1-k) +
 *            ∑_{i+j+k=n-1} a(i)*a(j)*a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

/* Multiply two seq_t values safely. Use 128‑bit intermediate if available and
   seq_t is an integral type; otherwise rely on the type's operator*. */
#if defined(__cplusplus)
static inline seq_t mul_seq(seq_t a, seq_t b)
{
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 t = (unsigned __int128)a * (unsigned __int128)b;
        return (seq_t)t;
    } else {
        return a * b;
    }
}

static inline seq_t add_seq(seq_t a, seq_t b)
{
    if constexpr (std::is_integral<seq_t>::value) {
        unsigned __int128 t = (unsigned __int128)a + (unsigned __int128)b;
        return (seq_t)t;
    } else {
        return a + b;
    }
}
#else
static inline seq_t mul_seq(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__)
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
#else
    return a * b;
#endif
}

static inline seq_t add_seq(seq_t a, seq_t b)
{
#if defined(__SIZEOF_INT128__)
    return (seq_t)((unsigned __int128)a + (unsigned __int128)b);
#else
    return a + b;
#endif
}
#endif

/* Cached sequence values and their convolution array. */
static seq_t *a_vals = NULL;
static seq_t *conv2_vals = NULL;
static idx_t seq_len = 0;          /* number of values currently stored */

seq_t a(idx_t n)
{
    /* Ensure that values up to n are computed. */
    if (n >= seq_len) {
        idx_t new_len = n + 1;
        a_vals      = (seq_t*)realloc(a_vals,      new_len * sizeof(seq_t));
        conv2_vals  = (seq_t*)realloc(conv2_vals,  new_len * sizeof(seq_t));

        /* Zero‑initialize newly allocated slots. */
        for (idx_t i = seq_len; i < new_len; ++i) {
            a_vals[i]   = 0;
            conv2_vals[i] = 0;
        }

        if (seq_len == 0) {
            /* Initialize a(0)=1 and conv2[0]=a0*a0. */
            a_vals[0]   = 1;
            conv2_vals[0] = 1;
            seq_len = 1;
        }

        /* Compute values for indices seq_len .. n. */
        for (idx_t i = seq_len; i <= n; ++i) {
            if (i == 0) continue;   /* already set */

            /* conv3[i-1] = Σ_{k=0}^{i-1} a(k) * conv2[i-1-k] */
            seq_t conv3 = 0;
            for (idx_t k = 0; k < i; ++k) {
                conv3 = add_seq(conv3, mul_seq(a_vals[k], conv2_vals[i - 1 - k]));
            }

            /* term = (i-1) * conv2[i-1] + conv3 */
            seq_t term = add_seq(mul_seq((seq_t)(i - 1), conv2_vals[i - 1]), conv3);
            a_vals[i] = term;

            /* conv2[i] = Σ_{k=0}^{i} a(k) * a(i-k) */
            seq_t sum2 = 0;
            for (idx_t k = 0; k <= i; ++k) {
                sum2 = add_seq(sum2, mul_seq(a_vals[k], a_vals[i - k]));
            }
            conv2_vals[i] = sum2;
        }

        seq_len = new_len;
    }

    return a_vals[n];
}
