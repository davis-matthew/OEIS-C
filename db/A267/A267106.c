/*
 * A267106  Permutation of natural numbers: a(1) = 1, a(2n) = A080148(a(n)),
 *          a(2n+1) = A080147(a(n)).
 * https://oeis.org/A267106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1) = 1, a(2n) = A080148(a(n)), a(2n+1) = A080147(a(n));
 *            computed by walking the bits of n below the leading 1, using a
 *            doubling prime sieve
 * Range:     u8 n <= 190, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 97 terms through n = 9123 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t *c_idx[2] = {NULL, NULL}; /* [0]: primes 3 mod 4, [1]: primes 1 mod 4; 1-based prime index of each */
static size_t c_cnt[2] = {0, 0};
static size_t sieve_limit = 0;

static void rebuild_sieve(size_t limit)
{
    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    size_t i, j, pi = 0;
    if (!comp) return;
    free(c_idx[0]);
    free(c_idx[1]);
    c_idx[0] = (idx_t *)malloc((limit / 2 + 2) * sizeof(idx_t));
    c_idx[1] = (idx_t *)malloc((limit / 2 + 2) * sizeof(idx_t));
    c_cnt[0] = c_cnt[1] = 0;
    for (i = 2; i <= limit; i++) {
        if (comp[i]) continue;
        pi++;
        if (i * i <= limit)
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
        if ((i & 3) == 3) c_idx[0][c_cnt[0]++] = (idx_t)pi;
        else if ((i & 3) == 1) c_idx[1][c_cnt[1]++] = (idx_t)pi;
    }
    free(comp);
    sieve_limit = limit;
}

/* cls 0: A080148, cls 1: A080147 */
static seq_t step_fn(seq_t m, int cls)
{
    size_t k = (size_t)(long long)m;
    while (c_cnt[cls] < k) {
        size_t nl = sieve_limit ? sieve_limit * 2 : 1024;
        rebuild_sieve(nl);
    }
    return (seq_t)c_idx[cls][k - 1];
}

seq_t a(idx_t n)
{
    seq_t v = 1;
    int top = 0, i;
    idx_t t = n;
    if (n < 1) return 0;
    while (t > 1) { t >>= 1; top++; }
    for (i = top - 1; i >= 0; i--) {
        int bit = (int)((n >> i) & 1);
        v = step_fn(v, bit ? 1 : 0);
    }
    return v;
}
