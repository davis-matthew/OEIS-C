/*
 * A329958  Expansion of q^(-13/24) * eta(q^2)^3 * eta(q^3) * eta(q^6) /
 *          eta(q)^2 in powers of q.
 * https://oeis.org/A329958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k)^2 (1 – x^{2k}) (1
 *            – x^{3k}) (1 – x^{6k})
 * Range:     u8 n <= 9065, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* allocate two buffers for alternating convolutions */
    size_t sz = (size_t)n + 1;
    long long *buf[2];
    buf[0] = (long long*)calloc(sz, sizeof(long long));
    buf[1] = (long long*)calloc(sz, sizeof(long long));
    if (!buf[0] || !buf[1]) { /* allocation failed */
        exit(1);
    }

    buf[0][0] = 1;              /* initial constant term 1 */
    int cur = 0, nxt = 1;

    /* offsets and coefficients of the 10 terms in each factor G_k(x) */
    static const idx_t offs[10] = {0, 1, 3, 4, 6, 7, 9, 10, 12, 13};
    static const long long cffs[10] = {1, 2, -3, -3, 1, -1, 3, 3, -2, -1};

    for (idx_t k = 1; k <= n; ++k) {
        /* reset next buffer */
        memset(buf[nxt], 0, sz * sizeof(long long));

        for (int t = 0; t < 10; ++t) {
            idx_t e = k * offs[t];
            if (e >= sz) continue;          /* term does not affect desired order */

            long long c = cffs[t];
            for (idx_t i = e; i < sz; ++i) {
                buf[nxt][i] += buf[cur][i - e] * c;
            }
        }
        /* swap buffers */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = (seq_t)buf[cur][n];
    free(buf[0]);
    free(buf[1]);
    return result;
}
