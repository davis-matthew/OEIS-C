/*
 * A371353  Irregular triangular array: row n shows the positions of fractions
 *          having denominator n in the array defined in A371280.
 * https://oeis.org/A371353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, frac, more
 * Formula:   a(n) = position in A371280's generation order of the reduced
 *            fraction k/r, where row r lists k = 1..r-1 (row 1 is 1/1) and
 *            r(r-1)/2 < n-1 <= r(r+1)/2 with k = n-1-r(r-1)/2. Computed by
 *            lazily generating T (1/1 in T; x/(1+y) in T) breadth-first,
 *            i-major then j-minor, with a hash table
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

typedef struct {
    seq_t num;
    seq_t den;
} A371353_frac;

static A371353_frac *A371353_fr = NULL;
static size_t A371353_cap = 0, A371353_len = 0;
static size_t *A371353_tab = NULL;   /* stores index+1, 0 = empty */
static size_t A371353_tsize = 0;
static size_t A371353_old = 1, A371353_gi = 0, A371353_gj = 0;
static bool A371353_ready = false;

static size_t A371353_hash(seq_t num, seq_t den)
{
    unsigned long long a = (unsigned long long)(num % (seq_t)1048573);
    unsigned long long b = (unsigned long long)(den % (seq_t)1048573);
    unsigned long long h = a * 1000003ULL + b * 7919ULL + (a ^ (b << 3));
    h ^= h >> 17;
    h *= 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)h;
}

/* returns index+1 or 0 */
static size_t A371353_find(seq_t num, seq_t den)
{
    size_t mask = A371353_tsize - 1;
    size_t h = A371353_hash(num, den) & mask;
    while (A371353_tab[h]) {
        size_t id = A371353_tab[h] - 1;
        if (A371353_fr[id].num == num && A371353_fr[id].den == den) return id + 1;
        h = (h + 1) & mask;
    }
    return 0;
}

static void A371353_tab_insert(size_t id)
{
    size_t mask = A371353_tsize - 1;
    size_t h = A371353_hash(A371353_fr[id].num, A371353_fr[id].den) & mask;
    while (A371353_tab[h]) h = (h + 1) & mask;
    A371353_tab[h] = id + 1;
}

static void A371353_rehash(size_t nsize)
{
    free(A371353_tab);
    A371353_tab = (size_t *)calloc(nsize, sizeof(size_t));
    if (!A371353_tab) abort();
    A371353_tsize = nsize;
    for (size_t i = 0; i < A371353_len; ++i) A371353_tab_insert(i);
}

static void A371353_init(void)
{
    if (A371353_ready) return;
    A371353_cap = 64;
    A371353_fr = mem_new(A371353_frac, A371353_cap);
    if (!A371353_fr) abort();
    A371353_fr[0].num = (seq_t)1;
    A371353_fr[0].den = (seq_t)1;
    A371353_len = 1;
    A371353_rehash(1024);
    A371353_old = 1; A371353_gi = 0; A371353_gj = 0;
    A371353_ready = true;
}

/* one (i,j) step of the generation */
static void A371353_step(void)
{
    if (A371353_gi >= A371353_old) {
        A371353_old = A371353_len;
        A371353_gi = 0;
        A371353_gj = 0;
    }
    seq_t ni = A371353_fr[A371353_gi].num, di = A371353_fr[A371353_gi].den;
    seq_t nj = A371353_fr[A371353_gj].num, dj = A371353_fr[A371353_gj].den;
    seq_t s = nj + dj;
    seq_t g1 = gcd(ni, s);
    seq_t g2 = gcd(dj, di);
    if (g1 == (seq_t)0) g1 = (seq_t)1;
    if (g2 == (seq_t)0) g2 = (seq_t)1;
    seq_t num = (ni / g1) * (dj / g2);
    seq_t den = (di / g2) * (s / g1);

    if (++A371353_gj >= A371353_old) {
        A371353_gj = 0;
        ++A371353_gi;
    }

    if (A371353_find(num, den)) return;
    if (A371353_len >= A371353_cap) {
        size_t nc = A371353_cap * 2;
        A371353_frac *t = mem_resize(A371353_frac, A371353_fr, nc);
        if (!t) abort();
        A371353_fr = t;
        A371353_cap = nc;
    }
    A371353_fr[A371353_len].num = num;
    A371353_fr[A371353_len].den = den;
    A371353_tab_insert(A371353_len);
    ++A371353_len;
    if (A371353_len * 2 > A371353_tsize) A371353_rehash(A371353_tsize * 2);
}

static idx_t A371353_gcdi(idx_t a, idx_t b)
{
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A371353_init();

    idx_t r, k;
    if (n == 1) {
        r = 1; k = 1;
    } else {
        idx_t m = n - 1;
        r = 2;
        while (r * (r - 1) / 2 < m) ++r;
        k = m - (r - 1) * (r - 2) / 2;
    }
    idx_t g = A371353_gcdi(k, r);
    seq_t num = (seq_t)(k / g);
    seq_t den = (seq_t)(r / g);

    for (;;) {
        size_t p = A371353_find(num, den);
        if (p) return (seq_t)p;
        if (A371353_len > 30000000u) return (seq_t)0;
        A371353_step();
    }
}
