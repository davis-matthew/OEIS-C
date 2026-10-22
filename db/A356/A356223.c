/*
 * A356223  Position of n-th appearance of 2n in the sequence of prime gaps
 *          (A001223). If 2n does not appear at least n times, set a(n) = -1.
 * https://oeis.org/A356223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence A356223.
   For a given n ≥ 1, the function returns the position of the
   n‑th occurrence of the gap value 2·n in the sequence of prime gaps
   (A001223).  If 2·n occurs fewer than n times, the function
   generates further primes until the condition is met.
   The algorithm is a straightforward generation of primes and
   storage of the gaps; no pre‑computed table is used.  This works
   for any seq_t that can be constructed from an unsigned integer. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- types and constants ----------------------------------- */
typedef uint64_t idx_t;   /* index type (fixed by the build) */

/* ---------- structures -------------------------------------------- */
struct gap_array {
    idx_t *pos;   /* 1‑based positions where this gap occurs */
    size_t len;   /* number of stored positions   */
    size_t cap;   /* current allocation capacity   */
};

/* ---------- static global state ----------------------------------- */
static uint64_t *primes  = NULL;   /* list of discovered primes       */
static size_t   primes_len = 0;
static size_t   primes_cap = 0;
static uint64_t last_prime  = 2;     /* previous prime in the sequence */
static uint64_t next_candidate = 3;  /* next odd number to test       */

static struct gap_array *gap_arrays = NULL;  /* gap_arrays[gap] -> positions  */
static size_t            gap_arrays_len = 0;
static size_t            gap_arrays_cap = 0;

static idx_t current_gap_index = 0;  /* 1‑based index of the last gap */

/* ---------- helper functions ------------------------------------- */
static void ensure_primes_cap(size_t new_cap)
{
    if (primes_cap >= new_cap) return;
    size_t cap = primes_cap == 0 ? 16 : primes_cap;
    while (cap < new_cap) cap <<= 1;

    uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
    if (!tmp && cap) abort();
    primes = tmp;
    primes_cap = cap;
}

static void ensure_gap_array(idx_t gap)
{
    if (gap_arrays_len > gap) return;
    size_t new_len = gap + 1;
    struct gap_array *tmp = (struct gap_array *)
        realloc(gap_arrays, new_len * sizeof(struct gap_array));
    if (!tmp && new_len) abort();
    for (size_t i = gap_arrays_len; i < new_len; ++i) {
        tmp[i].pos = NULL;
        tmp[i].len = 0;
        tmp[i].cap = 0;
    }
    gap_arrays = tmp;
    gap_arrays_len = new_len;
}

static void append_position(idx_t gap, idx_t pos)
{
    ensure_gap_array(gap);
    struct gap_array *ga = &gap_arrays[gap];
    if (ga->len == ga->cap) {
        size_t new_cap = ga->cap == 0 ? 4 : ga->cap << 1;
        idx_t *tmp = (idx_t *)realloc(ga->pos, new_cap * sizeof(idx_t));
        if (!tmp && new_cap) abort();
        ga->pos = tmp;
        ga->cap = new_cap;
    }
    ga->pos[ga->len++] = pos;
}

/* ---------- prime generation ------------------------------------- */
static void generate_next_prime(void)
{
    uint64_t cand = next_candidate;
    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p) break;         /* p² > cand */
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            /* record the new prime and its preceding gap */
            uint64_t newp = cand;
            idx_t gap = (idx_t)(newp - last_prime);
            ++current_gap_index;
            append_position(gap, current_gap_index);

            /* store the new prime */
            ensure_primes_cap(primes_len + 1);
            primes[primes_len++] = newp;
            last_prime = newp;
            next_candidate = newp + 2;   /* after a prime the next odd */
            break;
        }
        cand += 2;                         /* skip evens */
    }
}

/* ---------- main function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;   /* undefined for n = 0 */

    /* Initialise prime storage if not already done */
    if (!primes) {
        ensure_primes_cap(16);
        primes[0] = 2;
        primes_len = 1;
        last_prime = 2;
        next_candidate = 3;
    }

    idx_t target_gap = 2 * n;
    ensure_gap_array(target_gap);

    /* Grow the prime/gap list until we have at least n occurrences of 2·n */
    while (gap_arrays[target_gap].len < (size_t)n) {
        generate_next_prime();
    }

    return (seq_t)gap_arrays[target_gap].pos[n - 1];
}
