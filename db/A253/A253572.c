/*
 * A253572  Rectangular array A read by upward antidiagonals in which row A(n)
 *          is the sequence of all numbers divisible by no prime exceeding
 *          prime(n).
 * https://oeis.org/A253572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Idea:  A(1,1)=1.  Row r is the ascending list of all numbers whose
   prime factors lie in the first r primes (hamming numbers for that
   prime set).  The OEIS sequence is read along upward antidiagonals:
   diag d contains d terms – (d,1),(d-1,2),…,(1,d).  For a given n
   we find the diagonal d and the position inside it; then we fetch
   the required entry from row d.  Only modest amounts of data (≤~141
   primes, ≤141 elements per row) are required for n≤10000.  The
   algorithm is O(n) in the worst case. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_PRIME_R 200   /* enough for n up to about 10000 */

/* Small prime sieve – built on demand.  primes[0] = 2. */
static uint64_t primes[MAX_PRIME_R];
static size_t prime_count = 0;

static void
ensure_primes_upto(size_t r)
{
    if (r <= prime_count)
        return;
    uint64_t cand = (prime_count == 0) ? 2 : primes[prime_count - 1] + 1;
    while (prime_count < r) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes[prime_count++] = cand;
        }
        ++cand;
    }
}

/* Cached rows of Hamming numbers for each r (1‑based). */
static seq_t *row_cache[MAX_PRIME_R + 1] = {0};
static size_t row_len[MAX_PRIME_R + 1]   = {0};

static void
grow_row_cache(size_t r, size_t needed)
{
    if (needed <= row_len[r])
        return;
    seq_t *new_arr = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!new_arr)
        return;       /* allocation failed – caller will return 0 */

    new_arr[0] = (seq_t)1;
    size_t *idx = (size_t *)malloc(r * sizeof(size_t));
    if (!idx) {
        free(new_arr);
        return;
    }
    for (size_t i = 0; i < r; ++i)
        idx[i] = 0;

    for (size_t pos = 1; pos < needed; ++pos) {
        seq_t min_val = (seq_t)primes[0] * new_arr[idx[0]];
        for (size_t j = 1; j < r; ++j) {
            seq_t cand = (seq_t)primes[j] * new_arr[idx[j]];
            if (cand < min_val)
                min_val = cand;
        }
        new_arr[pos] = min_val;
        for (size_t j = 0; j < r; ++j) {
            seq_t cand = (seq_t)primes[j] * new_arr[idx[j]];
            if (cand == min_val)
                ++idx[j];
        }
    }
    free(idx);

    free(row_cache[r]);
    row_cache[r] = new_arr;
    row_len[r] = needed;
}

/* Locate the diagonal d and the coordinates (row,col) for position n. */
static void
antidiagonal_coords(idx_t n, idx_t *row, idx_t *col)
{
    if (n <= 1) {          /* n==1 gives row=1,col=1 */
        *row = 1;
        *col = 1;
        return;
    }
    /* Find smallest d such that d*(d+1)/2 >= n. */
    idx_t lo = 1, hi = 2;
    while ((hi * (hi + 1)) / 2 < n)
        hi *= 2;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;                 /* diagonal index (1‑based)   */
    idx_t prev = ((d - 1) * d) / 2;
    idx_t offset = n - prev - 1;  /* 0‑based offset in diagonal */
    *row = d - offset;            /* row counting from 1 at top   */
    *col = offset + 1;            /* column counting from 1 left  */
}

/* Main OEIS entry function. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;        /* undefined for n<1, return 0 as placeholder */

    idx_t r, c;
    antidiagonal_coords(n, &r, &c);

    if (r == 0)
        r = 1;
    if (c == 0)
        c = 1;

    ensure_primes_upto((size_t)r);
    grow_row_cache((size_t)r, (size_t)c);

    if (row_cache[(size_t)r])
        return row_cache[(size_t)r][c - 1];
    return 0; /* should never happen, but keeps compiler happy */
}
