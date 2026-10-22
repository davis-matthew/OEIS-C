/*
 * A387284  Expansion of 1/((1-3*x) * (1-7*x))^(7/2).
 * https://oeis.org/A387284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*a(n) = (10*n+25)*a(n-1) - 21*(n+5)*a(n-2) (n>1); a(0)=1,
 *            a(1)=35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)35;

    seq_t a_prev2 = (seq_t)1;
    seq_t a_prev1 = (seq_t)35;
    seq_t a_curr;                         /* will hold a(i) during the loop */

#ifdef __cplusplus
    using std::is_integral;
    for (idx_t i = 2; i <= n; ++i) {
        if constexpr (is_integral<seq_t>::value) {
            /* Integral seq_t (e.g. uint64_t) – use __int128 for safety. */
            unsigned __int128 f1 = (unsigned __int128)10 * (unsigned __int128)i + 25;
            unsigned __int128 f2 = (unsigned __int128)21 * ((unsigned __int128)i + 5);
            unsigned __int128 t1 = f1 * (unsigned __int128)a_prev1;
            unsigned __int128 t2 = f2 * (unsigned __int128)a_prev2;
            unsigned __int128 num = t1 - t2;
            a_curr = (seq_t)(num / (unsigned __int128)i);
        } else {
            /* arbitrary‑precision seq_t – use its own arithmetic. */
            seq_t f1 = (seq_t)10 * (seq_t)i + (seq_t)25;
            seq_t f2 = (seq_t)21 * ((seq_t)(i + 5));
            seq_t t1 = f1 * a_prev1;
            seq_t t2 = f2 * a_prev2;
            seq_t num = t1 - t2;
            a_curr = num / (seq_t)i;
        }
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
#else
    /* Pure C: seq_t is a built‑in unsigned integer type. */
    for (idx_t i = 2; i <= n; ++i) {
        unsigned __int128 f1 = (unsigned __int128)10 * (unsigned __int128)i + 25;
        unsigned __int128 f2 = (unsigned __int128)21 * ((unsigned __int128)i + 5);
        unsigned __int128 t1 = f1 * (unsigned __int128)a_prev1;
        unsigned __int128 t2 = f2 * (unsigned __int128)a_prev2;
        unsigned __int128 num = t1 - t2;
        a_curr = (seq_t)(num / (unsigned __int128)i);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
#endif

    return a_curr;
}
