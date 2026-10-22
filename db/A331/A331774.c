/*
 * A331774  a(n) = Sum_{-n<i<n, -n<j<n, gcd{i,j}=3} (n-|i|)*(n-|j|).
 * https://oeis.org/A331774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331774: a(n) = Σ_{-n<i<n, -n<j<n, gcd(i,j)=3} (n-|i|)*(n-|j|) */

#include <stdint.h>

static inline int gcd_local(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n <= 3) return 0;          // first non‑zero term starts at n = 4

    int Umax = (int)((n - 1) / 3);  /* maximum |u| or |v| with i=3u, j=3v */

    for (int u = -Umax; u <= Umax; ++u) {
        int i_int = 3 * u;                        /* i = 3u  (signed)   */
        int abs_i = i_int < 0 ? -i_int : i_int;   /* |i|  (signed)     */
        seq_t wi = (seq_t)n - (seq_t)abs_i;        /* (n-|i|)           */

        for (int v = -Umax; v <= Umax; ++v) {
            int j_int = 3 * v;
            int abs_j = j_int < 0 ? -j_int : j_int;

            /* gcd(i,j)=3 ⇔ 3|i,3|j and gcd(|i|/3,|j|/3)=1 */
            int g = gcd_local(u, v);              /* u and v are |i|/3 and |j|/3 up to sign */
            if (g == 1) {
                seq_t wj = (seq_t)n - (seq_t)abs_j;
                sum += wi * wj;
            }
        }
    }
    return sum;
}
