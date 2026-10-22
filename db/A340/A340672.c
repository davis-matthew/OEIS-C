/*
 * A340672  Number of repeated applications of the '4x+-1' operation needed to
 *          reach 1 from n, or -1 if 1 is never reached.
 * https://oeis.org/A340672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = 1 + a(f(n)) for n > 1 with a(1)=0. f(n) = 4*n-1 if n % 3
 *            == 1, 4*n+1 if n % 3 == 2, n/3 if n % 3 == 0. The function
 *            returns the number of steps to reach 1 from n (or -1 if no such
 *            path exists)
 * Range:     u8 n <= 8855, u16 n <= 8855, u32 n <= 8855, u64 n <= 8855,
 *            u128 n <= 8855, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define HASH_SIZE (1<<20)   /* 1,048,576 entries, should be enough for n <= 10k */

static idx_t hash_keys[HASH_SIZE];
static seq_t hash_vals[HASH_SIZE];

static inline bool hash_get(idx_t key, seq_t *out) {
    if (key == 0) return false;   /* 0 is never stored */
    size_t i = (size_t)key & (HASH_SIZE - 1);
    while (hash_keys[i] != 0) {
        if (hash_keys[i] == key) {
            *out = hash_vals[i];
            return true;
        }
        i = (i + 1) & (HASH_SIZE - 1);
    }
    return false;
}

static inline void hash_put(idx_t key, seq_t val) {
    if (key == 0) return;         /* 0 is not used as a key */
    size_t i = (size_t)key & (HASH_SIZE - 1);
    while (hash_keys[i] != 0) {
        if (hash_keys[i] == key) {
            hash_vals[i] = val;
            return;
        }
        i = (i + 1) & (HASH_SIZE - 1);
    }
    hash_keys[i] = key;
    hash_vals[i] = val;
}

seq_t a(idx_t n) {
    if (n == 1) return 0;

    static idx_t *chain = NULL;
    static size_t chain_cap = 0;
    size_t chain_len = 0;

    if (!chain) {
        chain_cap = 64;
        chain = (idx_t*)malloc(chain_cap * sizeof(idx_t));
    }

    idx_t cur = n;
    seq_t result = (seq_t)-1;

    for (;;) {
        seq_t val;
        if (hash_get(cur, &val)) {
            result = val;
            break;
        }
        if (cur == 1) {
            result = 0;
            break;
        }
        /* cycle detection: cur already in current path? */
        for (size_t i = 0; i < chain_len; ++i) {
            if (chain[i] == cur) {
                result = (seq_t)-1;
                goto backprop;
            }
        }
        /* store cur in path */
        if (chain_len >= chain_cap) {
            chain_cap <<= 1;
            chain = (idx_t*)realloc(chain, chain_cap * sizeof(idx_t));
        }
        chain[chain_len++] = cur;

        /* compute next state */
        idx_t mod = cur % 3;
        idx_t next;
        if (mod == 0) {
            next = cur / 3;
        } else if (mod == 1) {
            next = (idx_t)(((__uint128_t)4 * cur) - 1);
        } else { /* mod == 2 */
            next = (idx_t)(((__uint128_t)4 * cur) + 1);
        }
        cur = next;
    }

backprop:
    if (result == (seq_t)-1) {
        for (size_t i = 0; i < chain_len; ++i)
            hash_put(chain[i], (seq_t)-1);
    } else {
        seq_t steps = result;
        for (size_t i = chain_len; i > 0; ) {
            --i;
            steps = steps + 1;
            hash_put(chain[i], steps);
        }
    }

    seq_t ans;
    hash_get(n, &ans);
    return ans;
}
