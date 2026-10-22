/*
 * A349216  Number of ternary triples (u,v,w) with 1 <= u < v < w <= n.
 * https://oeis.org/A349216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{1 ≤ a < n} (n-a) × #{ 1 ≤ b < a : v3(a+b) >
 *            min(v3(a),v3(b)) } where v3(x) is the exponent of the highest
 *            power of 3 dividing x. This follows from the equivalent
 *            characterization v3((w-u)+(w-v)) > min(v3(w-u),v3(w-v)) with a =
 *            w-u and b = w-v
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int *ord3_cache = NULL;          /* cache of 3‑adic valuations */
static size_t ord3_cache_size = 0;      /* size of ord3_cache (max index stored) */

/* ensure that ord3_cache has valid values up to idx (inclusive) */
static void ensure_ord3(size_t idx)
{
    if (idx < ord3_cache_size)
        return;                       /* already enough */
    if (ord3_cache) {
        free(ord3_cache);
    }
    ord3_cache = (int *)malloc((idx + 1) * sizeof(int));
    ord3_cache_size = idx + 1;
    ord3_cache[0] = 0;
    for (size_t i = 1; i <= idx; ++i) {
        if (i % 3 == 0)
            ord3_cache[i] = ord3_cache[i / 3] + 1;
        else
            ord3_cache[i] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)        /* offset starts at 1, no triples below n=3 anyway */
        return (seq_t)0;

    /* ord3 needs values up to 2*n-2 at most (a+b) with a<n, b<a */
    ensure_ord3((size_t)(2 * (size_t)n));

    seq_t result = 0;
    for (idx_t a = 1; a <= n - 1; ++a) {
        int va = ord3_cache[(size_t)a];
        int cnt = 0;
        for (idx_t b = 1; b < a; ++b) {
            int vb = ord3_cache[(size_t)b];
            int minv = (va < vb) ? va : vb;
            int vsum = ord3_cache[(size_t)(a + b)];
            if (vsum > minv)
                ++cnt;
        }
        if (cnt != 0) {
            result += (seq_t)cnt * ((seq_t)(n - a));
        }
    }
    return result;
}
