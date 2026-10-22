/*
 * A306202  Matula-Goebel numbers of rooted semi-identity trees.
 * https://oeis.org/A306202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence consists of all positive integers n such that every
 *            prime index m > 1 dividing n is distinct and itself belongs to
 *            the sequence. This code generates the first 10 000 terms by
 *            scanning integers and checking the condition
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAX_VAL     200000   /* upper bound for value search   */
#define MAX_TERMS   10000    /* number of terms to precompute */

static seq_t seq[MAX_TERMS + 1];          /* 1‑based sequence array */
static size_t seq_len = 1;                /* current number of terms */
static unsigned char is_seq[MAX_VAL + 1]; /* 1 if value is in the sequence */

/* sieve data */
static int spf[MAX_VAL + 1];             /* smallest prime factor */
static int prime_index[MAX_VAL + 1];      /* prime → its index in prime list */
static int primes[200000];               /* prime list, 17984 < 200000 */
static size_t prime_cnt = 0;

/* one‑time initializer */
static void init_sequence(void)
{
    /* Chck if already initialised – not needed here but future‑proof. */
    static bool initialized = false;
    if (initialized)
        return;

    /* Linear sieve for SPF and prime list up to MAX_VAL. */
    spf[0] = 0;
    spf[1] = 1;
    for (int i = 2; i <= MAX_VAL; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt && primes[j] <= spf[i] &&
             (long long)i * primes[j] <= MAX_VAL; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    /* Map each prime to its 1‑based index. */
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_index[primes[i]] = (int)i + 1;

    /* Initial term a(1)=1. */
    seq[1] = (seq_t)1;
    memset(is_seq, 0, sizeof(is_seq));
    is_seq[1] = 1;

    /* Scan integers from 2 upwards, storing terms until we have
       MAX_TERMS entries. */
    for (int cand = 2; cand <= MAX_VAL && seq_len < MAX_TERMS; ++cand) {
        int temp = cand;
        bool ok = true;
        while (temp > 1) {
            int p = spf[temp];
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            int m = prime_index[p];
            if (m > 1) {                     /* ignore index 1 (prime 2) */
                if (cnt > 1 || !is_seq[m]) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            ++seq_len;
            seq[seq_len] = (seq_t)cand;
            is_seq[cand] = 1;
        }
    }
    initialized = true;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    init_sequence();
    if (n <= seq_len)
        return seq[(size_t)n];
    /* If n exceeds precomputed range, return 0 (undefined). */
    return (seq_t)0;
}
