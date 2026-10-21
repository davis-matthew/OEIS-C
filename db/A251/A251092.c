/*
 * A251092  a(n) is the number of primes in the n-th group of consecutive
 *          primes among the odd numbers.
 * https://oeis.org/A251092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes in the n‑th group of consecutive odd
 *            primes * where successive primes differ by 2 (twin‑prime
 *            chains). Groups are maximal; * the first group is 3,5,7 so
 *            a(1)=3. This implementation stores only the * run lengths as
 *            small integers and caches them, so it works for * arbitrary...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cap_uint8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* works in C11 and C++17, no use of new, class or template. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_cap_uint8.h"

/* --- helpers for dynamic arrays of small integers ---------------------------------- */

static void ensure_cap_uint64(uint64_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
    while (newcap < needed) newcap <<= 1;
    *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
    *cap = newcap;
}

/* --- state for the sequence ------------------------------------------------------ */

struct seq_state {
    uint8_t    *lens;         /* completed run lengths */
    size_t      cap;          /* capacity of lens */
    size_t      len;          /* number of runs stored */

    uint64_t    last_prime;   /* last prime processed */
    uint64_t    current_len;  /* length of the run being built */

    uint64_t  *primes;        /* list of odd primes discovered */
    size_t      primes_len;
    size_t      primes_cap;
};
static struct seq_state state;

/* initialize static state on first use */
static void init_state(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    state.lens = NULL;   state.cap = 0;   state.len = 0;
    state.last_prime = 7;          /* last prime of first run */
    state.current_len = 3;         /* length of first run */

    state.primes = NULL; state.primes_cap = 0; state.primes_len = 0;
    /* store first three odd primes */
    ensure_cap_uint64(&state.primes, &state.primes_cap, state.primes_len + 3);
    state.primes[state.primes_len++] = 3;
    state.primes[state.primes_len++] = 5;
    state.primes[state.primes_len++] = 7;
}

/* trial division odd prime generator using stored primes */
static uint64_t next_prime_after(uint64_t after)
{
    init_state();  /* ensure primes list is ready */
    uint64_t cand = after + 1;
    if ((cand & 1ull) == 0) cand++;   /* only odd candidates */

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < state.primes_len; ++i) {
            uint64_t p = state.primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            /* append to primes list */
            ensure_cap_uint64(&state.primes, &state.primes_cap, state.primes_len + 1);
            state.primes[state.primes_len++] = cand;
            return cand;
        }
        cand += 2;   /* skip even numbers */
    }
}

/* extend precomputed runs up to covering n runs */
static void ensure_runs(idx_t n)
{
    init_state();
    while (state.len < (size_t)n) {
        uint64_t next = next_prime_after(state.last_prime);
        uint64_t diff = next - state.last_prime;
        if (diff == 2ull) {
            /* still inside the same run */
            state.current_len += 1;
        } else {
            /* current run ends */
            ensure_cap_uint8(&state.lens, &state.cap, state.len + 1);
            state.lens[state.len++] = (uint8_t)state.current_len;
            state.current_len = 1;   /* start new run with this prime */
        }
        state.last_prime = next;
    }
}

/* --------------------------------------------------------------------- */
/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, guard against n==0 */
    ensure_runs(n);
    return (seq_t)state.lens[n - 1];
}
