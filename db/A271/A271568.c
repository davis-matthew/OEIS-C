/*
 * A271568  Squarefree semiprimes n such that phi(n) - 1 is prime.
 * https://oeis.org/A271568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3423, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A271568: Squarefree semiprimes n such that φ(n) - 1 is prime.
 * The code keeps a global list of found terms and extends it as necessary.
 * It uses a smallest prime factor (SPF) array to test each integer for the
 * semiprime property and a fast primality test for φ(n)-1.
 *
 * The sequence is generated in increasing order of n and fully memoised
 * so any order of calls to a(idx_t) is supported.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

static uint64_t *spf = NULL;        /* smallest prime factor array (uint64_t) */
static size_t   spf_limit = 0;      /* current maximum integer with SPF known */

static seq_t  *seq = NULL;          /* generated sequence values (seq_t) */
static size_t seq_cap = 0;          /* capacity of seq array */
static size_t seq_len = 0;          /* number of terms currently stored */

static idx_t cur = 4;                /* next integer to test (>=4) */

/*
 * Extend the SPF table to cover at least 'min_limit'.
 */
static void extend_spf(idx_t min_limit)
{
    if (min_limit <= spf_limit) return;
    idx_t new_limit = spf_limit;
    if (new_limit == 0) new_limit = 100000;              /* start size */
    while (new_limit < min_limit) new_limit *= 2;        /* exceed needed limit */

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/*
 * Append a new term to the global sequence.
 */
static void append_term(seq_t value)
{
    grow_seq(&seq, &seq_cap, seq_len + 1);
    seq[seq_len++] = value;
}

/*
 * Generate sequence terms up to at least index 'need'.
 * This routine is called lazily every time a(idx_t n) is requested.
 */
static void ensure_terms(idx_t need)
{
    while (seq_len < need) {
        /* make sure SPF table covers 'cur' */
        extend_spf(cur);

        /* check if cur is a squarefree semiprime */
        uint64_t m = cur;
        uint64_t p = spf[m];
        if (p == 0) p = m;                 /* should not happen for m>=2 */
        uint64_t q = m / p;
        if (q != p && spf[q] == q) {       /* distinct primes */
            uint64_t phi = (p - 1) * (q - 1);
            if (phi > 0 && is_prime(phi - 1)) {
                append_term((seq_t)m);
            }
        }
        ++cur;
    }
}

/*
 * seq_t a(idx_t n)
 *
 * Returns the n-th term (1‑based) of sequence A271568.
 */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for 0, guard anyway */
    ensure_terms(n);                /* generate terms up to n */
    return seq[n - 1];
}
