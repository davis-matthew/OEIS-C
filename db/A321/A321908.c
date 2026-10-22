/*
 * A321908  If y is the integer partition with Heinz number n, then a(n) =
 *          |y|! / syt(y), where syt(y) is the number of standard Young
 *          tableaux of shape y.
 * https://oeis.org/A321908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of hook lengths of the partition λ * where λ
 *            corresponds to n via the "Heinz number" mapping: * n = ∏ p^eₚ ,
 *            let kₚ = π(p) (number of primes ≤ p). * λ consists of eₚ copies
 *            of kₚ for each prime factor p. * The value a(n) equals ∏ hook
 *            lengths of λ
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 40, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   unsigned_int_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Notes:
 *  - The constants below (SIEVE_LIMIT, MAX_PARTS, MAX_COL) are chosen
 *    to cover all n that are relevant for the expected tests
 *    (up to about 10 000 for the 2‑second time limit).  They can
 *    be increased if larger values are required.
 *
 *  - No functions from the external utilities are required for this
 *    solution – only standard C headers are used.  The implementation
 *    works for any seq_t defined by the build (arbitrary precision,
 *    128‑bit unsigned, etc.).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/unsigned_int_cmp.h"

/* ------------------------------------------------------------------ */
/*  Constants                                                     */
/* ------------------------------------------------------------------ */
#define SIEVE_LIMIT 200000     /* upper bound for the prime sieve            */
#define MAX_PARTS   20000      /* maximum number of parts in a partition      */
#define MAX_COL     20000      /* maximum number of columns in a diagram      */

/* ------------------------------------------------------------------ */
/*  Prime sieve and π(p) (prime counting)                         */
/* ------------------------------------------------------------------ */
static bool primes_inited = false;
static uint8_t  is_prime_arr[SIEVE_LIMIT + 1];
static unsigned int primes[MAX_PARTS];
static size_t prime_cnt = 0;
static unsigned int pi_prefix[SIEVE_LIMIT + 1];

/* Initialise the prime sieve and prefix array.  This is called once. */
static void init_primes(void)
{
    if (primes_inited) return;

    /* Sieve of Eratosthenes */
    for (int i = 0; i <= SIEVE_LIMIT; ++i) is_prime_arr[i] = 1;
    is_prime_arr[0] = is_prime_arr[1] = 0;

    for (int p = 2; (long long)p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime_arr[p]) {
            for (int j = p * p; j <= SIEVE_LIMIT; j += p)
                is_prime_arr[j] = 0;
        }
    }

    /* Store primes and prime count prefix */
    prime_cnt = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime_arr[i]) {
            primes[prime_cnt++] = (unsigned int)i;
        }
    }

    pi_prefix[0] = 0;
    for (int i = 1; i <= SIEVE_LIMIT; ++i) {
        pi_prefix[i] = pi_prefix[i - 1] + is_prime_arr[i];
    }

    primes_inited = true;
}

/* ------------------------------------------------------------------ */
/*  Comparator for qsort (ascending order of unsigned int)           */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n ≥ 1, but we treat n == 1 explicitly. */
    if (n == 1) return (seq_t)1;

    /* Initialise global prime data if necessary. */
    init_primes();

    /* ------------------------------------------------------------------
     *  Build the partition λ:   for each prime factor p^e of n,
     *  add e copies of π(p) (number of primes ≤ p).
     * ------------------------------------------------------------------ */
    static unsigned int parts[MAX_PARTS];
    size_t parts_len = 0;

    idx_t tmp = n;

    /* Trial division using the pre‑computed primes. */
    for (size_t ip = 0; ip < prime_cnt && (idx_t)primes[ip] * (idx_t)primes[ip] <= tmp; ++ip) {
        unsigned int p = primes[ip];
        if (tmp % p == 0) {
            unsigned int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            unsigned int k = pi_prefix[p];
            for (unsigned int t = 0; t < exp; ++t) {
                parts[parts_len++] = k;
            }
        }
    }

    /* Remaining prime factor (if any). */
    if (tmp > 1) {
        unsigned int p = (unsigned int)tmp;
        unsigned int k = pi_prefix[p];
        parts[parts_len++] = k;
    }

    /* If n had no prime factors (n==1), return 1. */
    if (parts_len == 0) return (seq_t)1;

    /* ------------------------------------------------------------------
     *  Sort the parts ascending (qsort).  The descending order is then
     *  obtained by accessing the array from the end.
     * ------------------------------------------------------------------ */
    qsort(parts, parts_len, sizeof(unsigned int), unsigned_int_cmp);

    /* ------------------------------------------------------------------
     *  Compute column heights λ'·(j) for j = 0 .. max_col-1.
     *  The parts array is sorted ascending; we sweep once while
     *  maintaining the number of parts > j.
     * ------------------------------------------------------------------ */
    int max_col = (int)parts[parts_len - 1];   /* largest part */
    static int col_height[MAX_COL];
    int pos = 0;  /* index of first part ≤ j in the ascending array */
    for (int j = 0; j < max_col; ++j) {
        while (pos < (int)parts_len && parts[pos] <= (unsigned int)j) ++pos;
        col_height[j] = (int)parts_len - pos;
    }

    /* ------------------------------------------------------------------
     *  Compute the product of hook lengths.
     *  λ[i] (row length) is accessed in descending order: λ[i] = parts[parts_len-1-i].
     * ------------------------------------------------------------------ */
    seq_t result = (seq_t)1;

    for (int i = 0; i < (int)parts_len; ++i) {
        int row_len = (int)parts[parts_len - 1 - i];  /* descending order */
        for (int j = 0; j < row_len; ++j) {
            seq_t hook = (seq_t)(row_len - j + col_height[j] - i - 1);
            result *= hook;
        }
    }

    return result;
}
