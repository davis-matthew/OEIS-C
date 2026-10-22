/*
 * A372146  The smallest number k for which exactly n of its divisors are
 *          digitally balanced numbers in base 3 (A049354).
 * https://oeis.org/A372146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k such that exactly n of its divisors are
 *            digitally balanced numbers in base 3 (OEIS A372146)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds a sieve that counts, for every integer, how many
   divisors it has in the set of digitally balanced numbers.  For each
   divisor count it keeps the first (smallest) number that reaches that
   count.  The answer for any requested n is therefore retrieved from
   the pre‑computed table. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_balanced.h"

/* Types defined by the build system. */
typedef idx_t idx_t;   /* unsigned integer type for indices   */
typedef seq_t seq_t;   /* unsigned integer type for values    */

/* ---------- internal state ---------- */
static idx_t        limit      = 0;   /* highest integer currently sieved */
static uint16_t    *divcnt     = NULL;/* divisor counts for balanced numbers */
static uint64_t    *balanced   = NULL;/* list of discovered balanced numbers */
static uint64_t    *next_mul   = NULL;/* next multiple ≥ limit for each b */
static size_t      bal_size   = 0;   /* count of discovered balanced numbers */
static size_t      bal_cap    = 0;   /* capacity of above arrays */

static seq_t      *answers    = NULL;/* answers[n] = minimal k for n balanced divisors */
static size_t      ans_cap    = 0;   /* capacity of answers array */
static bool        initialized= false;/* one‑time init flag */

/* ---------- helper functions ---------- */

/* Ensure answers array has at least 'need' capacity. */
static void ensure_ans_cap(idx_t need)
{
    while (need >= (idx_t)ans_cap) {
        size_t new_cap = ans_cap == 0 ? 8 : ans_cap * 2;
        seq_t *p = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
        if (!p) exit(EXIT_FAILURE);
        /* zero‐initialize new slots */
        memset(p + ans_cap, 0, (new_cap - ans_cap) * sizeof(seq_t));
        answers = p;
        ans_cap  = new_cap;
    }
}

/* Ensure space for at least one more balanced number. */
static void ensure_balanced_cap(void)
{
    if (bal_size < bal_cap) return;
    size_t new_cap = bal_cap == 0 ? 8 : bal_cap * 2;
    uint64_t *p_bal = (uint64_t *)realloc(balanced, new_cap * sizeof(uint64_t));
    uint64_t *p_nm  = (uint64_t *)realloc(next_mul,   new_cap * sizeof(uint64_t));
    if (!p_bal || !p_nm) exit(EXIT_FAILURE);
    balanced = p_bal;
    next_mul = p_nm;
    bal_cap   = new_cap;
}

/* One‑time initialization: set answer[0] = 1. */
static void init_once(void)
{
    if (initialized) return;
    ensure_ans_cap(0);
    answers[0] = (seq_t)1;          /* 1 has 0 balanced divisors */
    initialized = true;
}

/* Extend the sieve up to inclusive 'new_limit'. */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= limit) return;

    idx_t old_limit = limit;

    /* Resize divisor count array. */
    if (divcnt == NULL) {
        divcnt = (uint16_t *)calloc(new_limit + 1, sizeof(uint16_t));
        if (!divcnt) exit(EXIT_FAILURE);
    } else {
        uint16_t *tmp = (uint16_t *)realloc(divcnt, (new_limit + 1) * sizeof(uint16_t));
        if (!tmp) exit(EXIT_FAILURE);
        divcnt = tmp;
        memset(divcnt + old_limit + 1, 0,
               (new_limit - old_limit) * sizeof(uint16_t));
    }

    /* Propagate existing balanced numbers into the new region. */
    for (size_t i = 0; i < bal_size; ++i) {
        uint64_t b = balanced[i];
        uint64_t m = next_mul[i];
        while (m <= new_limit) {
            divcnt[m]++;
            m += b;
        }
        next_mul[i] = m; /* store next multiple beyond new_limit */
    }

    /* Scan new numbers for being balanced, add them, and update counts. */
    for (idx_t m = old_limit + 1; m <= new_limit; ++m) {
        if (is_balanced(m)) {
            ensure_balanced_cap();
            size_t idx = bal_size;
            balanced[idx] = (uint64_t)m;
            next_mul[idx] = (uint64_t)m + (uint64_t)m; /* next multiple after m */
            bal_size++;

            /* Count this balanced number as a divisor of its own multiples
               within the new limit.  The first multiple is the number itself. */
            uint64_t mul = (uint64_t)m;
            while (mul <= new_limit) {
                divcnt[mul]++;
                mul += (uint64_t)m;
            }
            /* Store the next multiple (after new_limit). */
            next_mul[idx] = mul;
        }
    }

    /* Fill the answers array with any new minimal values found in this range. */
    for (idx_t m = old_limit + 1; m <= new_limit; ++m) {
        uint16_t cnt = divcnt[m];
        idx_t id = cnt;
        if (id >= (idx_t)ans_cap) ensure_ans_cap(id);
        if (answers[id] == 0) answers[id] = (seq_t)m;
    }

    limit = new_limit;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    init_once();                   /* ensure answers[0] is set */

    if (n >= (idx_t)ans_cap) ensure_ans_cap(n);
    if (answers[n] != 0) return answers[n];

    /* Expand the sieve until the answer for n is found. */
    idx_t l = limit;
    if (l == 0) l = 1;
    while (answers[n] == 0) {
        idx_t new_limit = l * 2;
        if (new_limit <= l) new_limit = (idx_t)-1;  /* overflow guard */
        extend_sieve(new_limit);
        l = new_limit;
    }
    return answers[n];
}
