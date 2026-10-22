/*
 * A376654  Sorted positions of first appearances in the second differences of
 *          consecutive prime-powers exclusive (A246655).
 * https://oeis.org/A376654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376654:
 * a(n) = index of the first appearance of each new non‑zero second
 * difference in the sequence of second differences of consecutive prime
 * powers (A246655), sorted in increasing order.  The indices are
 * counted from 1: the first second difference (between the first three
 * prime powers) has index 1.  Values equal to zero are ignored.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Constants -------------------------------------------------------------- */

#define LIMIT        5000000         /* upper bound for prime powers         */
#define MAX_PRIMES   400000          /* enough for primes <= LIMIT          */
#define MAX_POWERS   800000          /* upper bound for number of prime powers */
#define MAX_AVALS    20000           /* space for a(n) values                */
#define MAX_SEEN     40000           /* space for seen second‑difference values */

/* Global storage ----------------------------------------------------------- */

static uint64_t primes[MAX_PRIMES];
static size_t   primes_cnt      = 0;

static uint64_t powers[MAX_POWERS];
static size_t   powers_cnt      = 0;

static seq_t    a_vals[MAX_AVALS];
static size_t   a_vals_cnt      = 0;

static seq_t    seen_vals[MAX_SEEN];
static size_t   seen_cnt        = 0;

/* Helper: compare two uint64_t values for qsort --------------------------- */

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

/* Build the list of prime powers up to LIMIT ----------------------------- */

static void build_prime_powers(void)
{
    /* ---------- sieve to get primes ---------------------------------- */
    bool *is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (is_prime == NULL) exit(1);
    for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;

    free(is_prime);

    /* ---------- generate prime powers -------------------------------- */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t val = p;
        while (val <= LIMIT) {
            powers[powers_cnt++] = val;
            if (val > LIMIT / p) break;
            val *= p;
        }
    }

    /* ---------- sort the generated prime powers ----------------------- */
    qsort(powers, powers_cnt, sizeof(uint64_t), cmp_uint64);
}

/* Build the a(n) table up to MAX_AVALS ----------------------------------- */

static void build_sequence(void)
{
    if (a_vals_cnt > 0) return;   /* already built */

    build_prime_powers();

    /* ---------- compute second differences and collect first indices --- */
    uint64_t last_D = 0;
    bool last_D_defined = false;
    uint64_t S_index = 0;

    for (size_t i = 1; i < powers_cnt; ++i) {
        uint64_t D = powers[i] - powers[i - 1];
        if (last_D_defined) {
            S_index++;
            int64_t diff = (int64_t)D - (int64_t)last_D;
            if (diff != 0) {
                seq_t val = (seq_t)diff;  /* wrap as unsigned if seq_t is unsigned */
                /* check if already seen */
                bool already = false;
                for (size_t k = 0; k < seen_cnt; ++k)
                    if (seen_vals[k] == val) { already = true; break; }

                if (!already) {
                    /* record first appearance */
                    if (seen_cnt < MAX_SEEN) seen_vals[seen_cnt++] = val;
                    if (a_vals_cnt < MAX_AVALS) a_vals[a_vals_cnt++] = S_index;
                }
            }
        }
        last_D = D;
        last_D_defined = true;
    }
}

/* Public interface ---------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* ulp for out of bounds; not part of sequence */
    build_sequence();
    if ((idx_t)n > a_vals_cnt) return 0;   /* too large for pre‑computed range */
    return a_vals[n - 1];
}
