/*
 * A359597  Indices k such that A358777(k) is odd, and k is not an odd
 *          semiprime.
 * https://oeis.org/A359597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A358777(k) odd and k not an odd semiprime.
 *            Computed by a sieve: parity of the Dirichlet inverse of A353557
 *            mod 2, b(1)=1, b(m) = sum_{d|m, d<m} A353557(m/d) b(d) mod 2
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *t359597_terms = NULL;
static size_t t359597_cnt = 0;
static idx_t t359597_limit = 0;

static void t359597_build(idx_t L)
{
    size_t sz = (size_t)L + 1;
    uint32_t *spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    uint8_t *om = (uint8_t *)malloc(sz);
    uint8_t *acc = (uint8_t *)malloc(sz);
    uint8_t *b = (uint8_t *)malloc(sz);
    idx_t *res = (idx_t *)malloc(sz * sizeof(idx_t) / 2 + 16);
    if (!spf || !om || !acc || !b || !res) exit(EXIT_FAILURE);
    for (size_t i = 0; i < sz; ++i) { spf[i] = 0; om[i] = 0; acc[i] = 0; b[i] = 0; }
    for (size_t i = 2; i < sz; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j < sz; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
        om[i] = (uint8_t)(om[i / spf[i]] + 1);
    }
    size_t cnt = 0;
    for (size_t m = 1; m < sz; ++m) {
        b[m] = (m == 1) ? 1 : (acc[m] & 1);
        if (b[m]) {
            for (size_t t = 3; m * t < sz; t += 2) {
                if ((om[t] & 1) == 0) acc[m * t] ^= 1;
            }
            bool oddsemi = (m & 1) && om[m] == 2;
            if (!oddsemi) res[cnt++] = (idx_t)m;
        }
    }
    free(spf); free(om); free(acc); free(b);
    free(t359597_terms);
    t359597_terms = res;
    t359597_cnt = cnt;
    t359597_limit = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (t359597_limit == 0) t359597_build(1 << 14);
    while (t359597_cnt < (size_t)n)
        t359597_build(t359597_limit * 2);
    return (seq_t)t359597_terms[n - 1];
}
