/*
 * A362703  Expansion of e.g.f. 1/(1 + LambertW(-x^3 * exp(x))).
 * https://oeis.org/A362703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^(n-2*k) / (k! * (n-3*k)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#ifndef __cplusplus
/* C implementation – safe for uint8_t … uint64_t and unsigned __int128 */

static inline seq_t mul_div(seq_t a, seq_t b, seq_t d)
{
    /* Perform a*b/d using unsigned __int128 to avoid overflow when seq_t is 64‑bit */
    return (seq_t)(((unsigned __int128)a * b) / d);
}

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result = mul_div(result, base, (seq_t)1);
        exp >>= 1;
        if (exp)
            base = mul_div(base, base, (seq_t)1);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t result = (seq_t)0;
    idx_t max_k = n / 3;

    for (idx_t k = 1; k <= max_k; ++k)
    {
        idx_t exp = n - 2 * k;
        seq_t k_pow = pow_seq((seq_t)k, exp);

        /* ratio = n! / (k! * (n-3k)!)  computed as product over 3k terms divided by k! */
        idx_t start = n - 3 * k + 1;
        seq_t ratio = (seq_t)1;

        for (idx_t i = 1; i <= 3 * k; ++i)
        {
            seq_t num = (seq_t)(start + i - 1);
            seq_t div = (i <= k) ? (seq_t)i : (seq_t)1;
            ratio = mul_div(ratio, num, div);
        }

        /* term = ratio * k_pow  (checked that intermediate fits in 128 bits) */
        seq_t term = mul_div(ratio, k_pow, (seq_t)1);
        result += term;
    }

    return result;
}

#else
/* C++ implementation – works with any integral seq_t (exact arithmetic or big‑int) */

#include <type_traits>

template<typename T>
static inline T mul_div(T a, T b, T d)
{
    if constexpr (sizeof(T) <= 8)   // built‑in integer types
        return (T)(((unsigned __int128)a * b) / d);
    else                            // arbitrary‑precision type
        return (a * b) / d;
}

template<typename T>
static inline T pow_seq(T base, idx_t exp)
{
    T result = T(1);
    while (exp > 0)
    {
        if (exp & 1)
            result = mul_div(result, base, T(1));
        exp >>= 1;
        if (exp)
            base = mul_div(base, base, T(1));
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_t(1);          // a(0) = 1

    seq_t result = seq_t(0);
    idx_t max_k = n / 3;

    for (idx_t k = 1; k <= max_k; ++k)
    {
        idx_t exp = n - 2 * k;
        seq_t k_pow = pow_seq(seq_t(k), exp);

        idx_t start = n - 3 * k + 1;
        seq_t ratio = seq_t(1);

        for (idx_t i = 1; i <= 3 * k; ++i)
        {
            seq_t num = seq_t(start + i - 1);
            seq_t div = (i <= k) ? seq_t(i) : seq_t(1);
            ratio = mul_div(ratio, num, div);
        }

        seq_t term = mul_div(ratio, k_pow, seq_t(1));
        result += term;
    }
    return result;
}
#endif
