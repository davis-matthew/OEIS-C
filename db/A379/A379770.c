/*
 * A379770  Irregular triangle T(j,k) read by rows: split the natural numbers
 *          m > 1 into groups of length 2^s and then sort each group in the
 *          order defined in A019565.
 * https://oeis.org/A379770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   row j (j>=1) lists m = 2^(j-1)+i for i = 1..2^(j-1)-1 sorted by
 *            increasing f(i) = A019565(i), followed by m = 2^j; a(0) = 1.
 *            Computed by sorting each row
 * Range:     u8 n <= 254, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 84 terms through n = 9644 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint32_t A379770_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

typedef struct {
    unsigned __int128 f;
    uint64_t i;
} A379770_item;

static int A379770_cmp(const void *x, const void *y)
{
    const A379770_item *p = (const A379770_item *)x;
    const A379770_item *q = (const A379770_item *)y;
    if (p->f < q->f) return -1;
    if (p->f > q->f) return 1;
    return 0;
}

static unsigned __int128 A379770_f(uint64_t n)
{
    unsigned __int128 r = 1;
    unsigned bit = 0;
    while (n) {
        if (n & 1ULL) r *= (unsigned __int128)A379770_primes[bit];
        n >>= 1;
        bit++;
    }
    return r;
}

static A379770_item *A379770_cache = 0;
static int A379770_cache_s = -1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    int j = 0;
    {
        idx_t t = n;
        while (t) { j++; t >>= 1; }
    }
    int s = j - 1;
    uint64_t size = (uint64_t)1 << s;
    uint64_t k = (uint64_t)n - size;

    if (k == size - 1) {
        return (seq_t)1 << j;
    }

    if (A379770_cache_s != s) {
        if (A379770_cache) { free(A379770_cache); A379770_cache = 0; }
        A379770_cache_s = -1;
        uint64_t cnt = size - 1;
        A379770_cache = (A379770_item *)malloc((size_t)cnt * sizeof(A379770_item));
        if (!A379770_cache) return (seq_t)0;
        for (uint64_t i = 1; i <= cnt; i++) {
            A379770_cache[i - 1].f = A379770_f(i);
            A379770_cache[i - 1].i = i;
        }
        qsort(A379770_cache, (size_t)cnt, sizeof(A379770_item), A379770_cmp);
        A379770_cache_s = s;
    }

    return ((seq_t)1 << s) + (seq_t)A379770_cache[k].i;
}
