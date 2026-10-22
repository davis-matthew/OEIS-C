/*
 * A317094  a(n) = (n + 1)^2 + n!*L_n(-1), where L_n(x) is the Laguerre
 *          polynomial.
 * https://oeis.org/A317094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1)^2 + n!*L_n(-1) , L_n is Laguerre polynomial
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* safe multiplication of a seq_t value by an unsigned long long.
   Uses 128‑bit arithmetic when seq_t is an unsigned integer type,
   otherwise falls back to ordinary multiplication. */
#ifdef __cplusplus
#include <type_traits>
template <typename T>
inline T safe_mul(const T &a, unsigned long long b)
{
    if constexpr (std::is_unsigned<T>::value)
    {
        return static_cast<T>((static_cast<unsigned __int128>(a) * b));
    }
    else
    {
        return a * static_cast<T>(b);
    }
}
#else
/* In C we assume seq_t is an unsigned integer type */
static inline seq_t safe_mul(seq_t a, unsigned long long b)
{
    return (seq_t)((unsigned __int128)a * b);
}
#endif

seq_t a(idx_t n)
{
    /* Handle the two smallest indices directly */
    if (n == 0)
        return (seq_t)2;      /* (0+1)^2 + 0!*L_0(-1) = 1 + 1 = 2 */
    if (n == 1)
        return (seq_t)6;      /* (1+1)^2 + 1!*L_1(-1) = 4 + 2 = 6 */

    /* b_k = k!*L_k(-1).  We compute it via the recurrence:
           b_0 = 1
           b_1 = 2
           for k >= 2: b_k = 2*k*b_{k-1} - (k-1)^2*b_{k-2}   */
    seq_t b_prev2 = (seq_t)1;   /* b_0 */
    seq_t b_prev1 = (seq_t)2;   /* b_1 */
    seq_t b_curr = 0;
    idx_t i;

    for (i = 2; i <= n; ++i)
    {
        /* temp1 = 2*i*b_{i-1}   */
        seq_t temp1 = safe_mul(b_prev1, (unsigned long long)(2 * i));
        /* temp2 = (i-1)^2*b_{i-2} */
        unsigned long long factor = (i - 1) * (i - 1);
        seq_t temp2 = safe_mul(b_prev2, factor);
        b_curr = temp1 - temp2;

        /* shift for next iteration */
        b_prev2 = b_prev1;
        b_prev1 = b_curr;
    }

    /* a(n) = (n+1)^2 + b_n */
    seq_t np1 = (seq_t)n + 1;
    seq_t a_val = np1 * np1 + b_prev1;
    return a_val;
}
