/*
 * A285577  Irregular triangle T(n,m) read by rows (n >= 1, 0 <= m <=
 *          Max(A001221([1..n]))), giving the number of integers in [1,n] with
 *          m distinct prime factors.
 * https://oeis.org/A285577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) returns the n‑th term of the irregular triangle * T(k,m)
 *            read row‑wise. Each row k (k ≥ 1) consists of the counts * of
 *            integers in [1,k] with 0,1,2,…,Ω(k) distinct prime factors, *
 *            where Ω(k) is the maximum number of distinct prime factors among
 *            * 1…k. a(n) is the sequence of all such counts...
 * Range:     u8 n <= 2158, u16 n <= 9502, u32 n <= 9502, u64 n <= 9502,
 *            u128 n <= 9502, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* dynamic data structures ------------------------------------------------- */

/* current maximum row that has been processed */
static size_t cur_row = 0;
/* flattened sequence terms */
static size_t seq_size = 0, seq_cap = 0;
static seq_t *seq_vals = NULL;
/* smallest prime factor table */
static size_t spf_limit = 0;
static uint32_t *spf = NULL;
/* frequency counts of distinct prime factor counts */
static size_t max_omega = 0, freq_size = 0;
static seq_t *freq = NULL;

/* helper to grow a array of uint32_t for smallest prime factors */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_limit) return;

    /* allocate a new table of the required size */
    size_t new_sz = limit + 1;
    uint32_t *new_spf = (uint32_t *)malloc(new_sz * sizeof(uint32_t));
    if (!new_spf) abort();

    /* initialise */
    for (size_t i = 0; i < new_sz; ++i) new_spf[i] = 0;
    if (limit >= 1) new_spf[1] = 1;

    /* simple sieve of Eratosthenes to fill spf[2..limit] */
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* helper to enlarge the freq array */
static void ensure_freq(size_t min_size)
{
    if (freq_size >= min_size) return;

    size_t new_sz = freq_size ? freq_size * 2 : 8;
    if (new_sz < min_size) new_sz = min_size;

    seq_t *tmp = (seq_t *)realloc(freq, new_sz * sizeof(seq_t));
    if (!tmp) abort();
    /* initialise new entries to zero */
    for (size_t i = freq_size; i < new_sz; ++i) tmp[i] = 0;
    freq = tmp;
    freq_size = new_sz;
}

/* helper to enlarge the flattened sequence array */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap = new_cap;
}

/* main function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, input 0 is unused */

    size_t target = (size_t)n;
    while (seq_size < target) {
        /* ---- generate next row --------------------------------------- */
        ++cur_row;                       /* new n (row index)            */
        ensure_spf(cur_row);              /* extend spf if needed         */

        /* compute number of distinct prime factors of cur_row */
        size_t temp = cur_row;
        size_t w = 0;
        while (temp > 1) {
            uint32_t p = spf[temp];
            ++w;
            while (temp % p == 0) temp /= p;
        }

        /* make sure freq[] can store index w */
        if (w >= freq_size) ensure_freq(w + 1);

        /* update maximum omega if necessary */
        if (w > max_omega) max_omega = w;

        /* increment the count of numbers with w distinct prime factors */
        freq[w] += 1;

        /* ---- append this row's counts to the flattened sequence ----- */
        ensure_seq_capacity(seq_size + max_omega + 1);
        for (size_t m = 0; m <= max_omega; ++m) {
            seq_vals[seq_size++] = freq[m];
        }
    }

    return seq_vals[n - 1];
}
