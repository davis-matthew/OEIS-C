/* multichoose: C(n+k-1, k).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULTICHOOSE_H
#define OEISC_UTILS_MULTICHOOSE_H
#include <stdbool.h>
#include <stdint.h>

/* multichoose(t, m) = C(t+m-1, m), with the factorial cancelled exactly first */
static seq_t multichoose(seq_t t, idx_t m) {
    seq_t num[300];
    idx_t j, d;
    seq_t r = 1;
    if (m == 0) return 1;
    for (j = 0; j < m; j++) num[j] = t + j;
    for (d = 2; d <= m; d++) {
        idx_t x = d;
        while (x > 1) {
            idx_t p = 2;
            while (x % p != 0) p++;
            x /= p;
            for (j = 0; j < m; j++) {
                if (num[j] % p == 0) { num[j] = num[j] / p; break; }
            }
        }
    }
    for (j = 0; j < m; j++) r = r * num[j];
    return r;
}

#endif
