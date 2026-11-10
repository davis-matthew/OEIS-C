/*
 * A391802  Primes indexed by A391799.
 * https://oeis.org/A391802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A391799(n)), where A391799 lists the k with
 *            prime(2k+2)-prime(2k) > prime(2k+1)-prime(2k-1); computed by a
 *            doubling sieve and a scan
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A391802_pr = NULL;
static size_t A391802_pc = 0;
static uint64_t *A391802_ks = NULL;
static size_t A391802_kc = 0;
static uint64_t A391802_lim = 0;

static void A391802_grow(void)
{
    uint64_t lim = A391802_lim < 256 ? 256 : A391802_lim * 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t i, j;
    size_t cnt = 0, k;
    if (!comp)
        return;
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i)
                comp[j] = 1;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            cnt++;
    free(A391802_pr);
    free(A391802_ks);
    A391802_pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    A391802_ks = (uint64_t *)malloc((cnt / 2 + 1) * sizeof(uint64_t));
    if (!A391802_pr || !A391802_ks) {
        free(comp);
        free(A391802_pr);
        free(A391802_ks);
        A391802_pr = NULL;
        A391802_ks = NULL;
        A391802_pc = A391802_kc = 0;
        A391802_lim = 0;
        return;
    }
    cnt = 0;
    for (i = 2; i <= lim; i++)
        if (!comp[i])
            A391802_pr[cnt++] = i;
    free(comp);
    A391802_pc = cnt;
    A391802_lim = lim;
    A391802_kc = 0;
    /* prime(m) = pr[m-1]; need 2k+2 <= cnt */
    for (k = 1; 2 * k + 2 <= cnt; k++) {
        uint64_t left = A391802_pr[2 * k + 1] - A391802_pr[2 * k - 1];
        uint64_t right = A391802_pr[2 * k] - A391802_pr[2 * k - 2];
        if (left > right)
            A391802_ks[A391802_kc++] = k;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    while ((idx_t)A391802_kc < n) {
        size_t before = A391802_lim;
        A391802_grow();
        if (A391802_lim == 0 || A391802_lim == before)
            return (seq_t)0;
    }
    return (seq_t)A391802_pr[A391802_ks[n - 1] - 1];
}
