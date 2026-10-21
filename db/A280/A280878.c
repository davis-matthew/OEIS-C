/*
 * A280878  Occurrences of decrease of the probability density P(n) of coprime
 *          numbers k,m, satisfying 1 <= k <= a(n) and 1 <= m <= a(n), and
 *          a(n) congruent to 1(mod 2) and a(n) congruent to {3,9,21,27}(mod
 *          30).
 * https://oeis.org/A280878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2155, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280878:
   the nth integer m such that
     - m is odd and m ≡ 3,9,21,27 (mod 30),
     - let S(m) = Σ_{k=1..m} φ(k);
       P(m) = ((2·S(m))‑1) / m²;
     then P(m) < P(m‑1).
   This implementation pre‑computes φ(k) up to a large limit,
   keeps the cumulative sum, tests the inequality,
   and caches all qualifying terms for fast repeated queries. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper ---------------------------------------------------- */
static unsigned long long *phi_array = NULL;      /* φ(k) for k ≤ LIMIT */
static const unsigned int LIMIT = 2000000U;        /* safe upper bound */

/* ---------- state for lazy generation ------------------------------ */
static bool initialized = false;
static seq_t *terms = NULL;             /* cached a(n) values   */
static size_t terms_cap = 0;
static size_t terms_len = 0;
static unsigned int cur_candidate = 1;   /* next m to examine   */
static unsigned long long sum = 0;       /* Σ_{k=1..cur_candidate} φ(k) */
static unsigned long long sum_prev = 0;  /* Σ up to cur_candidate‑1 */

/* ---------- phi sieve ----------------------------------------------- */
static void init_phi(void)
{
    phi_array = (unsigned long long *)malloc((LIMIT + 1) *
                                              sizeof(unsigned long long));
    if (!phi_array) abort();

    for (unsigned int i = 0; i <= LIMIT; ++i)
        phi_array[i] = (unsigned long long)i;

    phi_array[0] = 0;
    phi_array[1] = 1;

    for (unsigned int p = 2; p <= LIMIT; ++p)
        if (phi_array[p] == (unsigned long long)p)        /* p prime */
            for (unsigned int j = p; j <= LIMIT; j += p)
                phi_array[j] -= phi_array[j] / p;

    initialized = true;
}

/* ---------- refill terms until we have at least n of them ---------------- */
static void ensure_terms(idx_t n)
{
    if (!initialized) init_phi();

    while (terms_len < (size_t)n) {
        /* Update cumulative sum */
        sum += phi_array[cur_candidate];

        if (cur_candidate > 1) {
            /* Test whether P(cur_candidate) < P(cur_candidate‑1) */
            unsigned __int128 lhs = (unsigned __int128)(2ULL * sum - 1ULL)
                                   * (unsigned __int128)(cur_candidate - 1)
                                   * (unsigned __int128)(cur_candidate - 1);
            unsigned __int128 rhs = (unsigned __int128)(2ULL * sum_prev - 1ULL)
                                   * (unsigned __int128)cur_candidate
                                   * (unsigned __int128)cur_candidate;
            bool decreased = lhs < rhs;

            if (decreased &&
                (cur_candidate & 1U) &&               /* odd */
                (cur_candidate % 30U == 3U ||
                 cur_candidate % 30U == 9U ||
                 cur_candidate % 30U == 21U ||
                 cur_candidate % 30U == 27U)) {
                /* Append to cached list */
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2U : 16U;
                    seq_t *tmp = (seq_t *)realloc(terms,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)cur_candidate;
            }
        }

        sum_prev = sum;
        ++cur_candidate;

        if (cur_candidate > LIMIT) {
            abort();   /* safety */
        }
    }
}

/* ---------- public interface ------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is unsigned; OEIS offset starts at 1 */
    ensure_terms(n);
    return terms[n - 1];
}
