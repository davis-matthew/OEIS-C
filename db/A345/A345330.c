/*
 * A345330  Composite numbers k for which m == 0 (mod k) is the only solution
 *          to m^(2^v(k-1)+1) == -m (mod k), where v(k) = A007814(k) is the
 *          2-adic valuation of k.
 * https://oeis.org/A345330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345330: odd composite numbers k such that for every prime factor p of k,
   the 2‑adic valuation v(p-1) ≤ v(k-1).  A term is obtained iff the only solution
   of m^(2^v(k-1)+1) ≡ -m (mod k) is m ≡ 0 (mod k).  The function generates the
   sequence by testing successive odd integers and storing those that satisfy
   the valuation condition.  Results are memoised so that calls with arbitrary
   order of n are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

static inline int v2_uint64(uint64_t x) {
    int cnt = 0;
    while ((x & 1) == 0) {
        ++cnt;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static struct {
        seq_t *cache;
        idx_t size;
        idx_t capacity;
        uint64_t next_candidate;
    } static_state = { NULL, 0, 0, 9 };

    /* Allocate cache on first use */
    if (static_state.cache == NULL) {
        static_state.capacity = 16;
        static_state.cache = (seq_t*)malloc(static_state.capacity * sizeof(seq_t));
        if (static_state.cache == NULL) abort();
    }

    /* Generate terms until we have at least n of them */
    while (static_state.size < n) {
        uint64_t k = static_state.next_candidate;

        /* Skip even numbers – all candidates are odd */
        if (!is_prime(k)) {
            int vk = v2_uint64(k - 1);

            /* Factor k.  We need only the distinct prime factors. */
            uint64_t pr[64], ex[64];
            size_t cnt = 0;
            factor_u64(k, pr, ex, &cnt);

            bool ok = true;
            for (size_t i = 0; i < cnt; ++i) {
                int vp = v2_uint64(pr[i] - 1);
                if (vp > vk) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                /* Grow cache if needed */
                if (static_state.size == static_state.capacity) {
                    static_state.capacity <<= 1; /* double */
                    seq_t *tmp = (seq_t*)realloc(static_state.cache,
                                                 static_state.capacity * sizeof(seq_t));
                    if (tmp == NULL) abort();
                    static_state.cache = tmp;
                }
                static_state.cache[static_state.size++] = k; /* implicit conversion to seq_t */
            }
        }

        /* Advance to next odd integer */
        static_state.next_candidate += 2;
    }

    return static_state.cache[n - 1];
}
