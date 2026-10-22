/*
 * A334438  Heinz numbers of all integer partitions sorted first by sum, then
 *          by length, and finally reverse-lexicographically.
 * https://oeis.org/A334438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the Heinz number of the integer partition that is the
 *            n-th (0‑based) in the order “first by sum, then by length,
 *            finally reverse‑lexicographically”
 * Range:     u8 n <= 65, u16 n <= 913, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all necessary partition counts
   up to a reasonable maximum sum, then uses them to locate the
   required partition and multiply the corresponding primes.   */

#include <stdint.h>
#include <stdbool.h>

/* ----------   Configuration   ---------------------------------------------- */
#define MAX_SUM   120          /* enough for n up to at least 100 000 */

/* ----------   Internal data structures   ----------------------------------- */
static bool initialized = false;
static const uint64_t  *part_pack;               /* unused, placeholder */
static uint64_t rest[MAX_SUM+1][MAX_SUM+1][MAX_SUM+1];

/* cumulative number of partitions up to each sum */
static uint64_t tot_part[MAX_SUM+1];
static uint64_t cum_part[MAX_SUM+1];

/* prime numbers indexed by 1,2,3,...  */
static uint64_t primes_by_index[MAX_SUM+1];

/* ----------   Helper functions   ------------------------------------------ */
static void init(void)
{
    /* ---- 1. Compute primes up to needed index --------------------------------- */
    const int LIMIT = 4000;          /* enough to get first MAX_SUM primes */
    bool is_prime[LIMIT+1];
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    int cnt = 0;
    for (int i = 2; i <= LIMIT && cnt < MAX_SUM; ++i)
        if (is_prime[i]) {
            ++cnt;
            primes_by_index[cnt] = (uint64_t)i;
        }

    /* ---- 2. Compute rest[r][l][max]  ---------------------------------------- */
    /* rest[r][l][max] = number of partitions of r into l parts,
       each part ≤ max, partitions considered as multisets (i.e. order
       nonincreasing).  */
    for (int maxP = 0; maxP <= MAX_SUM; ++maxP) {
        for (int r = 0; r <= MAX_SUM; ++r) {
            for (int l = 0; l <= MAX_SUM; ++l) {
                if (r == 0 && l == 0) {
                    rest[r][l][maxP] = 1;
                } else if (r == 0 || l == 0) {
                    rest[r][l][maxP] = 0;
                } else {
                    uint64_t val = rest[r][l][maxP-1];
                    if (maxP > 0 && r >= maxP && l >= 1)
                        val += rest[r - maxP][l-1][maxP];
                    rest[r][l][maxP] = val;
                }
            }
        }
    }

    /* ---- 3. Compute total and cumulative partitions for each sum -------------- */
    for (int s = 0; s <= MAX_SUM; ++s) {
        uint64_t tot = 0;
        if (s == 0) tot = 1;          /* empty partition */
        else
            for (int l = 1; l <= s; ++l)
                tot += rest[s][l][s];
        tot_part[s] = tot;
    }
    cum_part[0] = tot_part[0];
    for (int s = 1; s <= MAX_SUM; ++s)
        cum_part[s] = cum_part[s-1] + tot_part[s];

    initialized = true;
}

/* recursive construction of the k-th partition (0‑based) of sum 'rem',
   length 'len', with all parts ≤ maxP, following reverse‑lexicographic
   order.  The resulting parts are stored in parts[pos..]. */
static void build_part(idx_t rem, idx_t len, idx_t maxP, idx_t k,
                       uint64_t *parts, idx_t pos)
{
    if (len == 0) return;
    if (len == 1) {
        parts[pos] = rem;
        return;
    }

    idx_t upper = rem - (len - 1);        /* at least 1 for each remaining part */
    if (upper > maxP) upper = maxP;
    for (idx_t part = upper; part >= 1; --part) {
        uint64_t cnt = rest[rem - part][len - 1][part];
        if (k < cnt) {
            parts[pos] = part;
            build_part(rem - part, len - 1, part, k, parts, pos + 1);
            return;
        }
        k -= cnt;
    }
}

/* ----------   Main sequence function   ------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return (seq_t)1;   /* empty partition */

    /* find sum s where the n-th term lies */
    idx_t s = 0;
    while (s <= MAX_SUM && cum_part[s] <= n) s++;

    if (s > MAX_SUM)   /* should not happen for given constraints */
        return (seq_t)0;

    idx_t prev_cum = (s == 0) ? 0 : cum_part[s-1];
    idx_t offset = n - prev_cum;        /* 0‑based within partitions of sum s */

    /* find the length of the desired partition */
    idx_t L = 1;
    while (true) {
        uint64_t cnt = rest[s][L][s];
        if (offset < cnt) break;
        offset -= cnt;
        ++L;
    }

    /* construct the partition */
    uint64_t parts[MAX_SUM];
    build_part(s, L, s, offset, parts, 0);

    /* compute the Heinz number = product of primes at the given indices */
    seq_t result = 1;
    for (idx_t i = 0; i < L; ++i) {
        seq_t p = (seq_t)primes_by_index[(idx_t)parts[i]];  /* part ≥1 */
        result *= p;
    }
    return result;
}
