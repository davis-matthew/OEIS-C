/*
 * A250066  Number of length 7+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{13} Δ^k a(1) · C(n‑1, k) where Δ^k a(1) are the
 *            successive forward differences of the first 14 terms. The
 *            coefficients Δ^k a(1) are integers; the binomial terms are
 *            computed with exact arithmetic. This yields the exact value for
 *            any n for which the result fits into seq_t. For larg...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 30, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static const uint64_t first[14] = {
    21ULL, 21276ULL, 2398187ULL, 81138873ULL,
    1329866076ULL, 13468748489ULL, 96884873906ULL,
    539919000270ULL, 2469626954391ULL, 9653001714762ULL,
    33196888242117ULL, 102660657566831ULL,
    290292185100918ULL, 760444039774479ULL
};

static uint64_t delta[14];
static bool delta_initialized = false;

#ifdef __cplusplus
#include <type_traits>
static constexpr bool seq_t_is_class = std::is_class<seq_t>::value;
#endif

/* compute binomial coefficient C(n, k) exactly in seq_t */
static seq_t binom(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
#ifdef __cplusplus
        if constexpr (seq_t_is_class) {
            res = res * (n - k + i) / i;
        } else
#endif
        {
#if defined(__SIZEOF_INT128__) && !defined(NO_128)
            unsigned __int128 prod = (unsigned __int128)res * (unsigned __int128)(n - k + i);
            res = (seq_t)(prod / i);
#else
            res = res * (n - k + i) / i;
#endif
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (!delta_initialized) {
        uint64_t cur[14];
        for (int i = 0; i < 14; ++i) cur[i] = first[i];
        delta[0] = cur[0];
        for (int k = 1; k < 14; ++k) {
            int len = 14 - k;
            for (int i = 0; i < len; ++i)
                cur[i] = cur[i + 1] - cur[i];
            delta[k] = cur[0];
        }
        delta_initialized = true;
    }

    if (n <= 14) {
        return (seq_t)first[(size_t)(n - 1)];
    }

    seq_t sum = 0;

    for (int k = 0; k < 14; ++k) {
        if ((idx_t)k > n - 1u)
            break;
        seq_t bin = binom(n - 1, k);
#ifdef __cplusplus
        if constexpr (seq_t_is_class) {
            seq_t prod = (seq_t)delta[k] * bin;
            sum += prod;
        } else
#endif
        {
            unsigned __int128 prod = (unsigned __int128)delta[k] * (unsigned __int128)bin;
            sum += (seq_t)prod;
        }
    }
    return sum;
}
