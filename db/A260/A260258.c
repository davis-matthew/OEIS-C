/*
 * A260258  T(n,k) is the array read by rows, n>0 and k=1..q (with q = number
 *          of prime distinct divisors of n^2+1) giving the number of
 *          occurrences of the k-th prime divisor of n^2+1 counted from the
 *          prime divisors of m^2+1 for m=1..n.
 * https://oeis.org/A260258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1235, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 *   a(n) is the n-th element of the array T(r,k) read by rows,
 *   where T(r,k) counts the occurrences of the k-th prime divisor of r^2+1
 *   among all prime divisors of m^2+1 for m = 1..r.
 *
 * The sequence is generated on demand, memoising previously computed rows.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;                 /* memoised a(n) values, 0‑based */
static size_t seq_cap  = 0;
static size_t seq_len  = 0;

static uint64_t *prime_vals = NULL;      /* distinct primes seen so far */
static seq_t   *prime_cnts = NULL;       /* corresponding cumulative counts */
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;

static size_t current_row = 0;           /* number of rows already processed */

/*----------------------  Ensure capacities  ----------------------*/
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap > 0 ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_prime_cap(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap > 0 ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    prime_vals = (uint64_t *)realloc(prime_vals, new_cap * sizeof(uint64_t));
    prime_cnts = (seq_t   *)realloc(prime_cnts, new_cap * sizeof(seq_t));
    prime_cap = new_cap;
}

/* Find index of prime p in prime_vals; return prime_cnt if not present */
static size_t find_or_add_prime(uint64_t p)
{
    for (size_t i = 0; i < prime_cnt; ++i)
        if (prime_vals[i] == p) return i;

    /* new prime */
    ensure_prime_cap(prime_cnt + 1);
    prime_vals[prime_cnt] = p;
    prime_cnts[prime_cnt] = 0;
    ++prime_cnt;
    return prime_cnt - 1;
}

/* Factor n into distinct primes, store in out/ cnt.  Assumes n <= 2^64-1. */
static void factor_uint64(uint64_t n, uint64_t *out, size_t *cnt)
{
    size_t k = 0;
    if (n % 2 == 0) {
        out[k++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            out[k++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) out[k++] = n;
    *cnt = k;
}

/*----------------------  Main sequence routine  ----------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safest */

    /* Compute rows until we have at least n entries */
    while (seq_len < n) {
        /* Next row r = current_row + 1 */
        uint64_t r = (uint64_t)current_row + 1;
        uint64_t v = r * r + 1;          /* m^2 + 1 */

        /* Factor v into distinct primes */
        uint64_t factors[32];
        size_t fcount;
        factor_uint64(v, factors, &fcount);

        /* Ensure prime storage can hold new primes */
        ensure_prime_cap(prime_cnt + fcount);

        /* Increment count for each prime in this row */
        size_t idxs[32];                 /* temporary indices */
        for (size_t i = 0; i < fcount; ++i) {
            size_t idx = find_or_add_prime(factors[i]);
            prime_cnts[idx] += 1;
            idxs[i] = idx;                /* store to reuse for output */
        }

        /* Append T(r,k) values in ascending prime order */
        ensure_seq_cap(seq_len + fcount);
        for (size_t i = 0; i < fcount; ++i) {
            seq[seq_len++] = prime_cnts[idxs[i]];
        }

        ++current_row;
    }

    return seq[n - 1];
}
