// Regression: merge_sample_profiles segfaulted on baseline x86-64 builds (e.g. conda:
// -march=nocona -ftree-vectorize, GCC 13/14). The vectorizer emitted movdqa on the
// 8-mod-16 tetra_5prime_* arrays. Guarded by -fno-tree-vectorize on
// damage_estimation_update.cpp; this test catches a regression on any x86-64 runner
// built without -march (AVX builds use unaligned vmovdqu and never trip it).

#include "taph/frame_selector_decl.hpp"
#include "taph/sample_damage_profile.hpp"

#include <cstdio>
#include <memory>

int main() {
    // Heap, not stack: SampleDamageProfile is ~925 KiB.
    auto a = std::make_unique<taph::SampleDamageProfile>();
    auto b = std::make_unique<taph::SampleDamageProfile>();
    b->tetra_5prime_terminal[1] = 7;
    taph::FrameSelector::merge_sample_profiles(*a, *b);
    if (a->tetra_5prime_terminal[1] != 7) {
        std::fprintf(stderr, "FAIL: merge_sample_profiles did not sum tetra_5prime_terminal\n");
        return 1;
    }
    std::puts("merge_profiles OK");
    return 0;
}
