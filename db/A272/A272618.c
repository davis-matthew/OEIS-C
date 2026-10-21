/*
 * A272618  Irregular array read by rows: n-th row contains (in ascending
 *          order) the nondivisors 1 <= k < n such that all the prime divisors
 *          p of k also divide n.
 * https://oeis.org/A272618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = sequence defined by reading the irregular array A272618
 *            row-wise Each term is the next non‑divisor k of n such that
 *            every prime divisor of k also divides n. Rows that contain no
 *            such k contribute a single 0. (The array is read by rows; a(1)
 *            is the first element, etc.)
 * Range:     u8 n <= 1444, u16 n <= 9953, u32 n <= 9953, u64 n <= 9953,
 *            u128 n <= 9953, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static state for memoisation */
static seq_t  *seq      = NULL;  /* array of sequence terms */
static size_t  seq_cap  = 0;     /* current capacity of seq array      */
static size_t  seq_len  = 0;     /* number of terms stored in seq     */

/* SPF sieve for prime factorisation (up to a fixed limit) */
static uint64_t *spf    = NULL;  /* smallest prime factor for each ≤ limit */
static size_t   spf_cap  = 0;    /* current limit of spf table */
static const size_t SPF_LIMIT = 200000;   /* sufficient for n ≤ 10 000 */

/* initial row to start generating from */
static idx_t next_row = 1;

/* ------------------------------------------------------------------------- */
/* Utility: grow seq array if needed                                        */
static void
ensure_seq_size(size_t needed)
{
    if (seq_len + needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (seq_len + needed > new_cap) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------------- */
/* Initialise SPF table up to SPF_LIMIT                                      */
static void
init_spf(void)
{
    if (spf) return;                 /* already initialised */
    spf = (uint64_t *)calloc(SPF_LIMIT + 1, sizeof(uint64_t));
    if (!spf) return;
    for (size_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if ((size_t)i * i <= SPF_LIMIT) {
                for (size_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_cap = SPF_LIMIT;
}

/* ------------------------------------------------------------------------- */
/* Test whether n has more than one distinct prime divisor                 */
static bool
has_multiple_distinct_primes(idx_t n)
{
    if (n <= 3) return false;       /* 1,2,3 have ≤1 distinct prime */

    idx_t tmp   = n;
    idx_t lastp = 0;
    int   distinct = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        if (p == 0) p = tmp;        /* fallback for primes beyond SPF_LIMIT */
        if (p != lastp) {
            ++distinct;
            lastp = p;
            if (distinct > 1) return true;
        }
        tmp /= p;
    }
    return false;                   /* ≤1 distinct prime */
}

/* ------------------------------------------------------------------------- */
/* Append the next row of the irregular array to the seq array              */
static void
append_row(idx_t m)
{
    if (!has_multiple_distinct_primes(m)) {
        /* Row is empty → contribute a single 0 */
        ensure_seq_size(1);
        seq[seq_len++] = 0;
        return;
    }

    for (idx_t k = 2; k < m; ++k) {
        if (m % k == 0) continue;          /* k is a divisor → skip */

        idx_t tmp   = k;
        idx_t lastp = 0;
        bool ok = true;

        /* Check all distinct prime factors of k divide m */
        while (tmp > 1) {
            idx_t p = spf[tmp];
            if (p == 0) p = tmp;           /* on the off‑chance of k > SPF_LIMIT */

            if (p != lastp) {
                if (m % p != 0) { ok = false; break; }
                lastp = p;
            }
            tmp /= p;
        }
        if (!ok) continue;                 /* prime divisor of k not in m */

        ensure_seq_size(1);
        seq[seq_len++] = (seq_t)k;
    }
}

/* ------------------------------------------------------------------------- */
/* Public function required by the framework                                */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard – offset is 1 */

    init_spf();                           /* ensure SPF table ready */

    while (seq_len < (size_t)n) {
        append_row(next_row);
        ++next_row;
    }
    return seq[n - 1];
}
