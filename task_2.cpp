#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain {
public:
    Domain(const string& domain)
        : _domain(domain)
    {
        reverse(_domain.begin(), _domain.end());
        _domain.push_back('.');
    }

    bool operator==(const Domain& other) {
        return _domain == other._domain;
    }

    bool operator<(const Domain& other) const {
        return lexicographical_compare(_domain.begin(), _domain.end(), other._domain.begin(), other._domain.end());
    }

    bool IsSubdomain(const Domain& domain) const {
        return (_domain.find(domain._domain) == 0);
    }

    string GetDomain() const {
        return _domain;
    }

private:
    string _domain;
};

class DomainChecker {
public:
    template<typename Iterator>
    DomainChecker(Iterator first, Iterator last)
        : _forbidden_domains(first, last) {
        sort(_forbidden_domains.begin(), _forbidden_domains.end(),
             [](const Domain& lhs, const Domain& rhs){
                 return lhs.GetDomain() < rhs.GetDomain();
             });
        auto it = unique(_forbidden_domains.begin(), _forbidden_domains.end(),
                         [](const Domain& lhs, const Domain& rhs){
                             return rhs.IsSubdomain(lhs);
                         });
        _forbidden_domains.erase(it, _forbidden_domains.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(_forbidden_domains.begin(), _forbidden_domains.end(), domain);
        if (it == _forbidden_domains.begin())
            return false;
        else
            return domain.IsSubdomain(*prev(it));
    }

private:
    vector<Domain> _forbidden_domains;
};



template <typename Number>
vector<Domain> ReadDomains(istream& in, Number count) {
    vector<Domain> domains;
    domains.reserve(count);
    string domain;

    for (size_t i = 0; i < count; ++i) {
        getline(in, domain);
        domains.push_back(domain);
    }

    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    istringstream(line) >> num;

    return num;
}

void TestConstructor() {
    Domain domain("m.gdz.ru");
    assert(domain.GetDomain() == "ur.zdg.m.");
    cout<<"TestConstructor is OK"<<endl;
}

void TestIsSubdomain() {
    Domain domain("gdz.ru");
    assert(Domain("math.gdz.ru").IsSubdomain(domain));
    assert(Domain("history.gdz.ru").IsSubdomain(domain));
    assert(Domain("biology.gdz.ru").IsSubdomain(domain));
    assert(!Domain("freegdz.ru").IsSubdomain(domain));
    cout<<"TestIsSubdomain is OK"<<endl;
}

void TestEquality() {
    Domain domain1("m.maps.me");
    Domain domain2("m.maps.me");
    Domain domain3("m.gdz.ru");
    assert(domain1 == domain2);
    assert(!(domain1 == domain3));
    assert(!(domain2 == domain3));
    cout<<"TestEquality is OK"<<endl;
}


void TestDomainChecker() {
    const std::vector<Domain> _forbidden_domains{{"gdz.ru"}, {"maps.me"}, {"m.gdz.ru"}, {"com"}};
    DomainChecker domain_checker(_forbidden_domains.begin(), _forbidden_domains.end());
    assert(domain_checker.IsForbidden({"gdz.ru"}));
    assert(domain_checker.IsForbidden({"biology.gdz.ru"}));
    assert(domain_checker.IsForbidden({"history.gdz.ru"}));
    assert(domain_checker.IsForbidden({"math.gdz.ru"}));
    assert(!domain_checker.IsForbidden({"freegdz.ru"}));
    assert(domain_checker.IsForbidden({"gdz.com"}));
    assert(domain_checker.IsForbidden({"m.maps.me"}));
    assert(domain_checker.IsForbidden({"alg.m.gdz.ru"}));
    assert(domain_checker.IsForbidden({"maps.com"}));
    assert(!domain_checker.IsForbidden({"maps.ru"}));
    assert(!domain_checker.IsForbidden({"gdz.ua"}));
    cout<<"TestDomainChecker is OK"<<endl;
}

void UnitTests() {
    TestIsSubdomain();
    TestConstructor();
    TestEquality();
    TestDomainChecker();
}

int main() {
    UnitTests();

    cout<<"All tests passed"<<endl;

    const vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
