/*
 * A316502  Matula-Goebel numbers of unlabeled rooted trees with n nodes in
 *          which the branches of any node with more than one branch have
 *          empty intersection.
 * https://oeis.org/A316502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate sequence A316502 by checking each integer in
 * increasing order.  A number n is in the sequence iff
 *
 *   • n == 1
 *   • n is prime and its prime index (the position of n in the list of
 *     primes) is already in the sequence (i.e., that index value has
 *     appeared earlier),
 *   • n is composite, the set {m : prime(m) divides n} has greatest
 *     common divisor 1, and every such m already belongs to the
 *     sequence.
 *
 * The algorithm maintains a cache of the sequence values, a sieve of
 * primes up to a fixed LIMIT (here 200 000, sufficient for n ≤ 10 000),
 * and a mapping from a prime value to its 1‑based index in the prime
 * list.  The function `is_in_seq` implements the membership test.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

#define LIMIT 200000  /* sieve capacity; safe for n≤10000 */

/* Global state (lazily initialised) */
static bool          *sieve         = NULL;      /* is prime flag */
static idx_t          *primes        = NULL;      /* prime list */
static size_t          primeCnt      = 0;         /* number of primes */
static uint32_t       *primeToIdx    = NULL;      /* prime value → 1‑based index */

static bool           *seqFlag       = NULL;      /* value present in sequence */
static seq_t          *seqArray      = NULL;      /* stored terms */
static idx_t           seqCap        = 0;         /* capacity of seqArray */
static idx_t           seqSize       = 0;         /* number of computed terms */
static idx_t           processed     = 0;         /* last integer examined */
static bool            initialized   = false;

/* --------------------------------------------------------------------- */
static void init_globals(void)
{
    if (initialized) return;

    /* Allocate and build the sieve */
    sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    primeToIdx = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    for (idx_t i = 2; i <= LIMIT; ++i) sieve[i] = true;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) sieve[j] = false;
        }
    }

    /* Build prime list and mapping from prime value → index */
    primes = (idx_t *)malloc((LIMIT / 10 + 1) * sizeof(idx_t));  /* rough estimate */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            primeToIdx[i] = (uint32_t)(cnt + 1); /* 1‑based index */
            primes[cnt++] = i;
        }
    }
    primes = (idx_t *)realloc(primes, cnt * sizeof(idx_t));
    primeCnt = cnt;

    /* Prepare seqFlag array for values up to LIMIT */
    seqFlag = (bool *)calloc(LIMIT + 1, sizeof(bool));

    initialized = true;
}

/* --------------------------------------------------------------------- */
static bool is_in_seq(idx_t x)
{
    if (x == 1) return true;

    /* Prime check */
    if (x <= LIMIT && sieve[x]) {
        idx_t m = primeToIdx[x];   /* prime index */
        return seqFlag[m];
    }

    /* Composite number */
    idx_t temp = x;
    seq_t gcd_val = 0;
    idx_t idxList[32];
    size_t idxCnt = 0;

    /* Factorise using the pre‑computed prime list */
    for (size_t pi = 0; pi < primeCnt && primes[pi] * primes[pi] <= temp; ++pi) {
        idx_t p = primes[pi];
        if (temp % p == 0) {
            idx_t m = primeToIdx[p];
            idxList[idxCnt++] = m;
            if (gcd_val == 0)
                gcd_val = m;
            else
                gcd_val = gcd(gcd_val, (seq_t)m);
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        idx_t p = temp;
        idx_t m = primeToIdx[p];
        idxList[idxCnt++] = m;
        if (gcd_val == 0)
            gcd_val = m;
        else
            gcd_val = gcd(gcd_val, (seq_t)m);
    }

    if (gcd_val != (seq_t)1) return false;

    /* All prime indices must already appear in the sequence */
    for (size_t i = 0; i < idxCnt; ++i) {
        if (!seqFlag[idxList[i]]) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function.  Computes a(n) with memoisation and a
 * simple sieve‑based membership test. */
seq_t a(idx_t n)
{
    init_globals();

    if (n == 0) return (seq_t)0;   /* undefined for 0, but safe */

    if (seqCap < n) {
        seqCap = n;
        seqArray = (seq_t *)realloc(seqArray, seqCap * sizeof(seq_t));
    }

    while (seqSize < n) {
        ++processed;
        if (processed > LIMIT) {
            /* Should never happen for n ≤ 10000 with our LIMIT. */
            return (seq_t)0;
        }
        if (is_in_seq(processed)) {
            seqFlag[processed] = true;
            seqArray[seqSize++] = (seq_t)processed;
        }
    }
    return seqArray[n - 1];
}
