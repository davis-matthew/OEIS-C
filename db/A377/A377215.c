/*
 * A377215  Expansion of 1/(1 - 4*x^2/(1-x))^(5/2).
 * https://oeis.org/A377215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(n-1)*a(n-1) + (3*n+14)*a(n-2) - 2*(2*n-1)*a(n-3))/n
 *            with a(0)=1, a(1)=0, a(2)=10
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    #include <type_traits>
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)10;
    seq_t prev3 = (seq_t)1;   /* a(0)  */
    seq_t prev2 = (seq_t)0;   /* a(1)  */
    seq_t prev1 = (seq_t)10;  /* a(2)  */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t cur;
        if constexpr (std::is_integral<seq_t>::value) {
            unsigned __int128 t1 = (unsigned __int128)2 * (k - 1) * (unsigned __int128)prev1;
            unsigned __int128 t2 = (unsigned __int128)((unsigned __int128)3 * k + 14) * (unsigned __int128)prev2;
            unsigned __int128 t3 = (unsigned __int128)2 * ((unsigned __int128)2 * k - 1) * (unsigned __int128)prev3;
            unsigned __int128 sum = t1 + t2 - t3;
            cur = (seq_t)(sum / k);
        } else {
            cur = (seq_t)((seq_t)2 * (k - 1) * prev1
                          + ((seq_t)3 * k + 14) * prev2
                          - (seq_t)2 * ((seq_t)2 * k - 1) * prev3) / k;
        }
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
#else
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)10;
    seq_t prev3 = (seq_t)1;   /* a(0)  */
    seq_t prev2 = (seq_t)0;   /* a(1)  */
    seq_t prev1 = (seq_t)10;  /* a(2)  */
    for (idx_t k = 3; k <= n; ++k) {
        unsigned __int128 t1 = (unsigned __int128)2 * (k - 1) * (unsigned __int128)prev1;
        unsigned __int128 t2 = (unsigned __int128)((unsigned __int128)3 * k + 14) * (unsigned __int128)prev2;
        unsigned __int128 t3 = (unsigned __int128)2 * ((unsigned __int128)2 * k - 1) * (unsigned __int128)prev3;
        unsigned __int128 sum = t1 + t2 - t3;
        seq_t cur = (seq_t)(sum / k);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
#endif
}
