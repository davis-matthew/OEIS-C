/*
 * A353328  Number of divisors d of n for which A332823(d) is positive (+1).
 * https://oeis.org/A353328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A332823(d) > 0], where A332823(d) ==
 *            A048675(d) (mod 3), i.e. d = Product p_k^e_k contributes Sum
 *            e_k*(+1 if k odd, -1 if k even) (mod 3); computed by counting
 *            divisors per residue class mod 3 over the factorization of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9487 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A353328_comp = NULL; /* 1 = composite */
static uint64_t A353328_lim = 0;

static void A353328_ensure(uint64_t limit)
{
    if (limit <= A353328_lim) return;
    uint64_t nl = A353328_lim * 2;
    if (nl < limit) nl = limit;
    if (nl < 1024) nl = 1024;
    unsigned char *s = (unsigned char *)malloc((size_t)(nl + 1));
    for (uint64_t i = 0; i <= nl; i++) s[i] = 0;
    s[0] = 1; s[1] = 1;
    for (uint64_t i = 2; i * i <= nl; i++)
        if (!s[i])
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 1;
    free(A353328_comp);
    A353328_comp = s;
    A353328_lim = nl;
}

/* parity of the prime index of prime p (1 if index odd) */
static int A353328_index_odd(uint64_t p)
{
    A353328_ensure(p);
    uint64_t c = 0;
    for (uint64_t i = 2; i <= p; i++)
        if (!A353328_comp[i]) c++;
    return (int)(c & 1);
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t cnt[3] = {1, 0, 0};

    uint64_t ps[64];
    int es[64];
    int k = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            ps[k] = p; es[k] = e; k++;
        }
    }
    if (m > 1) { ps[k] = m; es[k] = 1; k++; }

    for (int i = 0; i < k; i++) {
        int w = A353328_index_odd(ps[i]) ? 1 : 2; /* +1 for odd index, -1 == 2 mod 3 for even */
        uint64_t nw[3] = {0, 0, 0};
        for (int r = 0; r < 3; r++) {
            for (int j = 0; j <= es[i]; j++) {
                nw[(r + w * j) % 3] += cnt[r];
            }
        }
        cnt[0] = nw[0]; cnt[1] = nw[1]; cnt[2] = nw[2];
    }
    return (seq_t)cnt[1];
}
