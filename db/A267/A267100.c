/*
 * A267100  Self-inverse permutation of natural numbers: a(1) = 1,
 *          a(A080147(n)) = A080148(n), a(A080148(n)) = A080147(n).
 * https://oeis.org/A267100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, if prime(n) mod 4 = 1 then a(n) =
 *            A080148(A267097(n)), otherwise a(n) = A080147(A267098(n)).
 *            Computed with a sieve and counting
 * Range:     u8 n <= 239, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 100 terms through n = 9616 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A267100_pr = NULL;
static size_t A267100_cnt = 0;
static uint64_t A267100_lim = 0;

static void A267100_grow(void)
{
    uint64_t lim = A267100_lim ? A267100_lim * 2 : 1024;
    char *s = (char *)malloc((size_t)lim + 1);
    if (!s) return;
    for (uint64_t i = 0; i <= lim; i++) s[i] = 1;
    s[0] = 0; s[1] = 0;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (s[i])
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 0;
    size_t c = 0;
    for (uint64_t i = 2; i <= lim; i++) if (s[i]) c++;
    uint32_t *np = (uint32_t *)malloc(c * sizeof(uint32_t));
    if (!np) { free(s); return; }
    size_t k = 0;
    for (uint64_t i = 2; i <= lim; i++) if (s[i]) np[k++] = (uint32_t)i;
    free(s);
    free(A267100_pr);
    A267100_pr = np;
    A267100_cnt = c;
    A267100_lim = lim;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    size_t N = (size_t)n;
    while (A267100_cnt < N) {
        size_t before = A267100_cnt;
        A267100_grow();
        if (A267100_cnt == before) return 0;
    }
    uint64_t c1 = 0, c3 = 0;
    for (size_t i = 0; i < N; i++) {
        uint32_t p = A267100_pr[i];
        if ((p & 3) == 1) c1++;
        else if ((p & 3) == 3) c3++;
    }
    bool one = ((A267100_pr[N - 1] & 3) == 1);
    uint32_t target = one ? 3 : 1;
    uint64_t need = one ? c1 : c3;
    for (;;) {
        uint64_t seen = 0;
        for (size_t i = 0; i < A267100_cnt; i++) {
            if ((A267100_pr[i] & 3) == target) {
                if (++seen == need) return (seq_t)(i + 1);
            }
        }
        size_t before = A267100_cnt;
        A267100_grow();
        if (A267100_cnt == before) return 0;
    }
}
