/*
 * A355079  Irregular triangle read by rows: the first row is 1, and the n-th
 *          row (n > 1) lists the factors f of n where n/f is prime (the
 *          maximal factors of n.)
 * https://oeis.org/A355079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A355079 – irregular triangle read by rows.
    Row 1: {1}
    For n > 1: list all factors f of n such that n/f is prime,
    i.e. for every distinct prime divisor p of n include f = n/p.
    The sequence a(n) is the n‑th entry when these rows are concatenated
    one after another.  This implementation uses a simple cumulative
    prefix sum to locate the row containing the requested index.  The
    same code works for any integer type seq_t (big integer, unsigned
    int, etc.) and for any idx_t being uint64_t.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- helper for counting distinct prime divisors (uint64_t) ------------- */
static seq_t count_distinct_prime_factors(uint64_t n) {
    seq_t cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            ++cnt;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) ++cnt;
    return cnt;
}

/* ---------- dynamic prefix array --------------------------------------------- */
static seq_t *prefix = NULL;          /* cumulative row counts, prefix[0] = 0          */
static size_t prefix_size = 0;        /* number of entries stored (>=1)              */
static size_t prefix_cap = 0;         /* allocated capacity                           */
static uint64_t next_row = 1;         /* next row number to be processed             */

static void grow_prefix(size_t needed) {
    if (needed <= prefix_cap) return;
    size_t new_cap = (prefix_cap == 0) ? 16 : prefix_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
    if (!tmp) exit(1); /* out of memory */
    prefix = tmp;
    prefix_cap = new_cap;
}

static void init_prefix(void) {
    if (prefix) return;          /* already initialized */
    grow_prefix(16);
    prefix[0] = 0;
    prefix_size = 1;
}

/* ---------- public function --------------------------------------------------- */
seq_t a(idx_t n) {
    /* initialise static state */
    init_prefix();

    /* enlarge the prefix array until it covers index n (seq_t comparison) */
    while (prefix[prefix_size - 1] < (seq_t)n) {
        uint64_t m = next_row;
        seq_t len = (m == 1) ? 1 : count_distinct_prime_factors(m);
        grow_prefix(prefix_size + 1);
        prefix[prefix_size] = prefix[prefix_size - 1] + len;
        ++prefix_size;
        ++next_row;
    }

    /* binary search for the row r with prefix[r-1] < n ≤ prefix[r] */
    size_t low = 1, high = prefix_size - 1;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (prefix[mid] >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t r = (uint64_t)low;                /* rows are 1‑based */
    seq_t offset = (seq_t)n - prefix[r - 1];    /* 1‑based position in row r */

    if (r == 1) return (seq_t)1;               /* row 1 is special */

    /* collect distinct prime divisors of r */
    uint64_t primes[64];
    size_t prime_cnt = 0;
    uint64_t temp = r;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[prime_cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && prime_cnt < 64) primes[prime_cnt++] = temp;

    /* compute the factors r/p and store them */
    seq_t factors[64];
    for (size_t i = 0; i < prime_cnt; ++i)
        factors[i] = (seq_t)(r / primes[i]);

    /* simple insertion sort – tiny array */
    for (size_t i = 1; i < prime_cnt; ++i) {
        seq_t key = factors[i];
        size_t j = i;
        while (j > 0 && factors[j - 1] > key) {
            factors[j] = factors[j - 1];
            --j;
        }
        factors[j] = key;
    }

    /* offset is seq_t; convert to size_t index */
    size_t idx = (size_t)(offset - 1);
    return factors[idx];
}
