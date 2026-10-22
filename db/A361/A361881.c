/*
 * A361881  Expansion of 1/(1 - 9*x/(1 + x))^(1/3).
 * https://oeis.org/A361881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A361881: Expansion of 1/(1 - 9*x/(1 + x))^(1/3).
   Recurrence: a(0)=1, a(1)=3, for n>1:
   n*a(n) = (7*n-4)*a(n-1) + 8*(n-2)*a(n-2). */

#if defined(__cplusplus)
#include <type_traits>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a_prev2 = 1;  /* a(0) */
    seq_t a_prev1 = 3;  /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        if constexpr (std::is_integral<seq_t>::value && (sizeof(seq_t) <= 8))
        {
            __int128 t1 = (__int128)a_prev1 * (7*i - 4);
            __int128 t2 = (__int128)a_prev2 * (8*(i-2));
            a_cur = (seq_t)((t1 + t2) / (__int128)i);
        }
        else
        {
            seq_t t1 = a_prev1 * (seq_t)(7*i - 4);
            seq_t t2 = a_prev2 * (seq_t)(8*(i-2));
            a_cur = (t1 + t2) / (seq_t)i;
        }
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
#else
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a_prev2 = 1;  /* a(0) */
    seq_t a_prev1 = 3;  /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        __int128 t1 = (__int128)a_prev1 * (7*i - 4);
        __int128 t2 = (__int128)a_prev2 * (8*(i-2));
        a_cur = (seq_t)((t1 + t2) / (__int128)i);
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
#endif
