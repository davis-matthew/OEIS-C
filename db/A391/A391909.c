/*
 * A391909  Numbers k such that -1 <= A195017(k) <= 1.
 * https://oeis.org/A391909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer k such that * |A257991(k) –
 *            A257992(k)| ≤ 1, * i.e. the difference between the counts of
 *            odd‑index and even‑index * prime factors of k (with
 *            multiplicity) is at most 1
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global data --------------------------------------------- */

static idx_t          sieve_limit   = 0;      /* largest number spf covers      */
static idx_t          *spf          = NULL;  /* smallest prime factor array    */
static uint8_t        *prime_parity = NULL;  /* 1 if prime index is odd, 0 otherwise */
static seq_t          *seq          = NULL;  /* stored sequence values          */
static size_t         seq_cap      = 0;      /* sequence array capacity         */
static size_t         seq_len      = 0;      /* number of elements stored       */
static idx_t          curr_value   = 1;      /* next integer to test            */

/* --- sieve construction ------------------------------------- */

static void build_sieve(idx_t limit)
{
    if (spf)      free(spf);
    if (prime_parity) free(prime_parity);

    sieve_limit = limit;
    spf  = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    prime_parity = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));

    /* initialise spf array */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;

    /* classic spf sieve */
    for (idx_t p = 2; (p * p) <= limit; ++p)
        if (spf[p] == p)                     /* p is prime      */
            for (idx_t i = p * p; i <= limit; i += p)
                if (spf[i] == i) spf[i] = p;

    /* compute parity of prime indices: 1 for odd, 0 for even */
    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i) {                   /* i is prime      */
            ++count;
            prime_parity[i] = (uint8_t)(count & 1);
        }
}

/* ensure sieve covers at least n */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit) return;
    idx_t new_limit = (sieve_limit > 0 ? sieve_limit : 128);
    while (new_limit < n) new_limit <<= 1;
    build_sieve(new_limit);
}

/* --- check the parity difference of k ----------------------- */

static int diff_parity(idx_t k)
{
    if (k == 1) return 0;
    int diff = 0;
    idx_t m = k;
    while (m > 1) {
        idx_t p = spf[m];
        int  cnt = 0;
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }
        diff += (prime_parity[p] ? cnt : -cnt);
    }
    return diff;
}

/* --- generate sequence up to needed terms ------------------- */

static void generate_up_to(size_t needed)
{
    while (seq_len < needed) {
        ensure_sieve(curr_value);
        if (abs(diff_parity(curr_value)) <= 1) {
            if (seq_len == seq_cap) {
                seq_cap = (seq_cap == 0 ? 256 : seq_cap << 1);
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)curr_value;
        }
        ++curr_value;
    }
}

/* --- required function ---------------------------------------- */

seq_t a(idx_t n)
{
    /* lazy initialization */
    if (!seq) {
        build_sieve(200000);                /* enough for n ≈ 10⁴      */
        seq_cap = 256;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        curr_value = 1;
    }

    generate_up_to((size_t)n);
    return seq[n - 1];
}
